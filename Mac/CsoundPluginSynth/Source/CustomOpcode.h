#ifndef CUSTOMOPCODE_H
#define CUSTOMOPCODE_H

#include "csound.h"
#include <csdl.h>
#include <vector>
#include <algorithm>
#include <cmath>

#ifndef FL
#define FL(x) ((MYFLT)(x))
#endif

/* =====================================================
   MONO MIDI STATE (Singleton)
   ===================================================== */
struct MonoMidiState
{
    /* ===== MIDI ===== */
    std::vector<int> stack;
        
    bool noteOn = false;

    double velAmp = 0.5;
    double amp = 0.0;

    /* ===== PITCH ===== */
    double freq = 0.0;
    double curFreq = 0.0;

    /* ===== GLIDE ===== */
    double glideS = 0.1;
    double glidePow = 1.0;
    double sr = 44100.0;

    double glidePos = 0.0;
    double glideInc = 0.0;
    double startFreq = 0.0;

    /* ===== ADSR ===== */
    double atk = 0.01;
    double dec = 0.1;
    double sus = 0.7;
    double rel = 0.2;

    double env = 0.0;
    double envCurve = 0.0;
    
    bool NoEnv = true;
    
    double atkPow = 0.0;
    double decPow = 0.0;
    double relPow = 0.0;

    enum EnvStage { OFF, ATTACK, DECAY, SUSTAIN, RELEASE, QUICK_RETRIG };
    EnvStage envStage = OFF;

    double atkInc = 0.0;
    double decInc = 0.0;
    double relInc = 0.0;
    
    /* ===== RETRIGGER ===== */
    bool ampRetrigger = false;

    /* ===== MIDI TRACKING (per ReadMidiData) ===== */
    int lastSentNote = -1;
    int lastVelocity = 64;
    
    /* ===== RELEASE TRACKING ===== */
    bool releaseCompleted = false;
    
    /* ===== PITCH BEND ===== */
    int pitchBendValue = 8192;
    double pitchBendSemitones = 2.0;
    
    /* ===== AFTERTOUCH ===== */
    int aftertouchValue = 0;
    
    /* ===== MIDI CC ===== */
    int ccValues[128];

    /* ===== COSTRUTTORE - inizializza CC ===== */
    MonoMidiState()
    {
        for (int i = 0; i < 128; i++)
            ccValues[i] = 0;
        
        ccValues[7] = 100;
        ccValues[10] = 64;
        ccValues[11] = 127;
    }

    /* ===== SINGLETON ===== */
    static MonoMidiState& get()
    {
        static MonoMidiState s;
        return s;
    }

    static double mtof(int n)
    {
        return 440.0 * pow(2.0, (n - 69) / 12.0);
    }

    /* ===== HELPER: Get current active note ===== */
    bool hasActiveNote() const
    {
        return !stack.empty();
    }

    int getLastNote() const
    {
        return stack.empty() ? -1 : stack.back();
    }

    int getLastVelocity() const
    {
        return lastVelocity;
    }
    
    /* ===== PITCH BEND HELPERS ===== */
    void setPitchBend(int lsb, int msb)
    {
        pitchBendValue = (msb << 7) | lsb;
    }
    
    double getPitchBendNormalized() const
    {
        return ((pitchBendValue - 8192.0) / 8192.0);
    }
    
    double getPitchBendSemitones() const
    {
        return getPitchBendNormalized() * pitchBendSemitones;
    }
    
    double getPitchBendRatio() const
    {
        return pow(2.0, getPitchBendSemitones() / 12.0);
    }
    
    /* ===== AFTERTOUCH HELPERS ===== */
    void setAftertouch(int value)
    {
        aftertouchValue = value;
    }
    
    double getAftertouchNormalized() const
    {
        return aftertouchValue / 127.0;
    }
    
    /* ===== CC HELPERS ===== */
    void setCC(int controller, int value)
    {
        if (controller >= 0 && controller < 128)
            ccValues[controller] = value;
    }
    
    int getCC(int controller) const
    {
        if (controller >= 0 && controller < 128)
            return ccValues[controller];
        return 0;
    }
    
    double getCCNormalized(int controller) const
    {
        return getCC(controller) / 127.0;
    }

    // /* ================= NOTE ON ================= */
    void noteOnEvent(int note, int vel)
    {
        // // Rimuovi la nota dallo stack se gi‡ presente (evita duplicati)
        // auto it = std::find(stack.begin(), stack.end(), note);
        // if (it != stack.end())
        // {
        //     stack.erase(it);
        // }
        
        stack.push_back(note);
        freq = mtof(note);
        lastVelocity = vel;

        if (!noteOn)
        {
            // Prima nota dopo il rilascio
            if (!NoEnv && envStage == RELEASE && env > 0.2)
            {
                // L'envelope Ë ancora alta durante release: fai retrig veloce
                amp = (vel / 127.0) * velAmp;
                startQuickRetrigger();
            }
            else
            {
                // Envelope bassa o off: parti normalmente
                amp = (vel / 127.0) * velAmp;
                curFreq = freq;
                glidePos = 1.0;
                
                if(!NoEnv)
                    startAttack();
            }
            
            noteOn = true;
        }
        else
        {
            if (ampRetrigger)
            {
                amp = (vel / 127.0) * velAmp;
                startQuickRetrigger();
            }
            
            startFreq = curFreq;
            startGlide();
        }
    }
    /* ================= NOTE OFF ================= */
    void noteOffEvent(int note)
    {
        stack.erase(std::remove(stack.begin(), stack.end(), note), stack.end());
        if (stack.empty())
        {
            noteOn = false;
            {
                if (envStage != RELEASE)
                    startRelease();
            }
        }
        else
        {
            freq = mtof(stack.back());
            startGlide();
        }
    }
    
    /* ================= GLIDE ================= */
    void startGlide()
    {
        double glideSamples = glideS * sr;

        if (glideSamples <= 1.0)
        {
            curFreq = freq;
            glidePos = 1.0;
            return;
        }

        startFreq = curFreq;
        glidePos = 0.0;
        glideInc = 1.0 / glideSamples;
    }

    inline double curveLinear(double t)
    {
        return t;
    }

    inline double curveExp(double t, double k)
    {
        return pow(t, k);
    }

    inline double curveLog(double t, double k)
    {
        return 1.0 - pow(1.0 - t, k);
    }

    void updateGlide(int ksmps)
    {
        if (!noteOn)
        {
            curFreq = freq;
            return;
        }

        for (int i = 0; i < ksmps; ++i)
        {
            if (glidePos >= 1.0)
            {
                curFreq = freq;
                break;
            }

            glidePos += glideInc;
            if (glidePos > 1.0)
                glidePos = 1.0;

            double t = glidePos;
            double shaped;

            if (std::abs(glidePow) < 1e-6)
                shaped = curveLinear(t);
            else if (glidePow > 0)
                shaped = curveExp(t, glidePow + 1.0);
            else
                shaped = curveLog(t, std::abs(glidePow) + 1.0);

            curFreq = startFreq + (freq - startFreq) * shaped;
        }
    }

    /* ================= ADSR ================= */
    
    void startAttack()
    {
        envStage = ATTACK;
    
        const double target = 1.0;
        const double distance = target - env;
    
        if (atk <= 0.0)
            atkInc = distance;
        else
            atkInc = distance / (atk * sr);
    
        releaseCompleted = false;
    }
    
    void startQuickFade()
    {
        envStage = QUICK_RETRIG;
        const double quickTime = 0.005; // 5ms ultra-veloce
        
        if (env <= 0.0)
        {
            relInc = 0.0;
        }
        else
        {
            relInc = env / (quickTime * sr);
        }
    }
    
    void startQuickRetrigger()
    {
        if (env < 0.01)
        {
            env = 0.0;
            startAttack();
        }
        else
        {
            startQuickFade();
        }
    }
    
    void startRelease()
    {
        if (envStage != RELEASE && envStage != OFF)
        {
            envStage = RELEASE;
        
            const double target = 0.0;
            const double distance = env - target;
        
            if (rel <= 0.0)
                relInc = distance;
            else
                relInc = distance / (rel * sr);
        
            releaseCompleted = false;
        }
    }
    
    std::vector<MYFLT> envBuf;
    
    inline MYFLT applyAttackCurve(MYFLT env)
    {
        if (std::abs(atkPow) < 1e-6)
            return curveLinear(env);
        else if (atkPow > 0)
            return curveExp(env, atkPow + 1.0);
        else
            return curveLog(env, std::abs(atkPow) + 1.0);
    }
    
    inline MYFLT applyDecayCurve(MYFLT env)
    {
        if (std::abs(decPow) < 1e-6)
            return curveLinear(env);
        else if (decPow > 0)
            return curveExp(env, decPow + 1.0);
        else
            return curveLog(env, std::abs(decPow) + 1.0);
    }
    
    inline MYFLT applyReleaseCurve(MYFLT env)
    {
        if (std::abs(relPow) < 1e-6)
            return curveLinear(env);
        else if (relPow > 0)
            return curveExp(env, relPow + 1.0);
        else
            return curveLog(env, std::abs(relPow) + 1.0);
        }
    
    void ensureBuffer(int ksmps)
    {
        if ((int)envBuf.size() != ksmps)
            envBuf.resize(ksmps);
    }
    
    void updateEnv(int ksmps)
    {
        ensureBuffer(ksmps);
    
        for (int i = 0; i < ksmps; ++i)
        {
            switch (envStage)
            {
                case ATTACK:
                    env += atkInc;
                    if (env >= 1.0)
                    {
                        env = 1.0;
                        envStage = DECAY;
                        decInc = (dec <= 0.0)
                            ? (1.0 - sus)
                            : ((1.0 - sus) / (dec * sr));
                    }
                    envBuf[i] = applyAttackCurve(env);
                    break;
    
                case DECAY:
                    env -= decInc;
                    if (env <= sus)
                    {
                        env = sus;
                        envStage = SUSTAIN;
                    }
                    envBuf[i] = applyDecayCurve(env);
                    break;
    
                case SUSTAIN:
                    envBuf[i] = env;
                    break;
    
                case RELEASE:
                    env -= relInc;
                    if (env <= 0.0)
                    {
                        env = 0.0;
                        envStage = OFF;
                        releaseCompleted = true;
                    }
                    envBuf[i] = applyReleaseCurve(env);
                    break;
    
                case QUICK_RETRIG:
                    env -= relInc;
                    if (env <= 0.0)
                    {
                        env = 0.0;
                        if (noteOn)
                            startAttack();
                        else
                        {
                            envStage = OFF;
                            releaseCompleted = true;
                        }
                    }
                    envBuf[i] = env; // Lineare per massima velocit‡
                    break;
    
                case OFF:
                default:
                    env = 0.0;
                    envBuf[i] = 0.0;
                    break;
            }
        }
    }
};

/* =====================================================
   OPCODE STRUCTURES
   ===================================================== */
typedef struct
{
    OPDS h;
    MYFLT *amp;
    MYFLT *velAmp;
    MYFLT *retrigger;
} MONOAMPMIDI;

typedef struct
{
    OPDS h;
    MYFLT *cps;
    MYFLT *glide;
    MYFLT *glidepow;
    MYFLT *pbmode;
    MYFLT *pbrange;
} MONOCPSMIDI;

typedef struct
{
    OPDS h;
    MYFLT *env;
    MYFLT *atk;
    MYFLT *dec;
    MYFLT *sus;
    MYFLT *rel;
    MYFLT *atkPow;
    MYFLT *decPow;
    MYFLT *relPow;
    MYFLT *retrigger;
} MONOADSR;

typedef struct
{
    OPDS h;
    MYFLT *notenum;
} MONONOTNUM;

typedef struct
{
    OPDS h;
    MYFLT *amp;
} MONONOTVEL;

typedef struct
{
    OPDS h;
    MYFLT *active;
} MONONOTEACTIVE;

typedef struct
{
    OPDS h;
    MYFLT *out;
    MYFLT *mode;
    MYFLT *range;
} MONOPITCHBEND;

typedef struct
{
    OPDS h;
    MYFLT *out;
} MONOAFTERTOUCH;

typedef struct
{
    OPDS h;
    MYFLT *out;
    MYFLT *ccnum;
    MYFLT *mode;
} MONOCC;

/* =====================================================
   MONOAMPMIDI OPCODE
   ===================================================== */
static int monoampmidi_init(CSOUND *csound, MONOAMPMIDI *p)
{
    MonoMidiState &s = MonoMidiState::get();
    s.NoEnv = true;
    s.sr = csound->GetSr(csound);
    s.velAmp = *p->velAmp;
    s.ampRetrigger = *p->retrigger;
    *p->amp = FL(0.0);
    *p->amp = (MYFLT)s.amp;
    return OK;
}

static int monoampmidi_perfKr(CSOUND *csound, MONOAMPMIDI *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->amp = (MYFLT)s.amp;
    return OK;
}

static int monoampmidi_perfSr(CSOUND *csound, MONOAMPMIDI *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->amp = (MYFLT)s.amp;
    return OK;
}

/* =====================================================
   MONOCPSMIDI OPCODE
   ===================================================== */
static int monocpsmidi_init(CSOUND *csound, MONOCPSMIDI *p)
{
    MonoMidiState &s = MonoMidiState::get();
    s.sr = csound->GetSr(csound);
    s.glideS = *p->glide;
    s.glidePow = *p->glidepow;
    
    if (*p->pbrange > 0)
        s.pitchBendSemitones = *p->pbrange;
    
    int pbmode = (int)(*p->pbmode);
    double freq = s.curFreq;
    
    if (pbmode == 1)
        freq = freq + (freq * s.getPitchBendRatio() - freq);
    else if (pbmode == 2)
        freq = freq * s.getPitchBendRatio();
    
    *p->cps = (MYFLT)freq;
    return OK;
}

static int monocpsmidi_perfKr(CSOUND *csound, MONOCPSMIDI *p)
{
    MonoMidiState &s = MonoMidiState::get();
    s.updateGlide(csound->GetKsmps(csound));
    
    int pbmode = (int)(*p->pbmode);
    double freq = s.curFreq;
    
    if (pbmode == 1)
        freq = freq + (freq * s.getPitchBendRatio() - freq);
    else if (pbmode == 2)
        freq = freq * s.getPitchBendRatio();
    
    *p->cps = (MYFLT)freq;
    return OK;
}

/* =====================================================
   MONOADSR OPCODE
   ===================================================== */
static int monoAdsr_init(CSOUND *csound, MONOADSR *p)
{
    MonoMidiState &s = MonoMidiState::get();
    s.NoEnv = false;
    s.sr = csound->GetSr(csound);
    s.atk = *p->atk;
    s.dec = *p->dec;
    s.sus = *p->sus;
    s.rel = *p->rel;
    s.atkPow = *p->atkPow;
    s.decPow = *p->decPow;
    s.relPow = *p->relPow;
    return OK;
}

static int monoAdsr_perfKr(CSOUND *csound, MONOADSR *p)
{
    MonoMidiState &s = MonoMidiState::get();
    uint32_t offset = p->h.insdshead->ksmps_offset;
    uint32_t early  = p->h.insdshead->ksmps_no_end;
    uint32_t nsmps  = csound->GetKsmps(csound);

    if (offset)
        memset(p->env, 0, offset * sizeof(MYFLT));

    if (early)
    {
        nsmps -= early;
        memset(&p->env[nsmps], 0, early * sizeof(MYFLT));
    }
    
    s.updateEnv(nsmps);

    for (uint32_t i = 0; i < nsmps; i++)
    {
        p->env[i] = (MYFLT)s.envBuf[i];
    }

    return OK;
}
static int monoAdsr_perfSr(CSOUND *csound, MONOADSR *p)
{
    MonoMidiState &s = MonoMidiState::get();
    uint32_t offset = p->h.insdshead->ksmps_offset;
    uint32_t early  = p->h.insdshead->ksmps_no_end;
    uint32_t nsmps  = csound->GetKsmps(csound);

    if (offset)
        memset(p->env, 0, offset * sizeof(MYFLT));

    if (early)
    {
        nsmps -= early;
        memset(&p->env[nsmps], 0, early * sizeof(MYFLT));
    }
    
    s.updateEnv(nsmps);

    for (uint32_t i = 0; i < nsmps; i++)
    {
        p->env[i] = (MYFLT)s.envBuf[i];
    }

    return OK;
}

/* =====================================================
   MONONOTNUM - Numero di nota MIDI corrente
   ===================================================== */
static int mononotnum_init(CSOUND *csound, MONONOTNUM *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->notenum = s.hasActiveNote() ? FL(s.getLastNote()) : FL(0.0);
    return OK;
}

static int mononotnum_perfKr(CSOUND *csound, MONONOTNUM *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->notenum = s.hasActiveNote() ? FL(s.getLastNote()) : FL(0.0);
    return OK;
}

/* =====================================================
   MONONOTVEL - Velocity MIDI corrente (0-1)
   ===================================================== */
static int mononotvel_init(CSOUND *csound, MONONOTVEL *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->amp = FL(s.getLastVelocity() / 127.0);
    return OK;
}

static int mononotvel_perfKr(CSOUND *csound, MONONOTVEL *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->amp = FL(s.getLastVelocity() / 127.0);
    return OK;
}

/* =====================================================
   MONONOTEACTIVE - 1 se nota attiva, 0 altrimenti
   ===================================================== */
static int mononoteactive_init(CSOUND *csound, MONONOTEACTIVE *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->active = s.hasActiveNote() ? FL(1.0) : FL(0.0);
    return OK;
}

static int mononoteactive_perfKr(CSOUND *csound, MONONOTEACTIVE *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->active = s.hasActiveNote() ? FL(1.0) : FL(0.0);
    return OK;
}

/* =====================================================
   MONOPITCHBEND - Pitch bend MIDI
   ===================================================== */
static int monopitchbend_init(CSOUND *csound, MONOPITCHBEND *p)
{
    MonoMidiState &s = MonoMidiState::get();
    
    if (*p->range > 0)
        s.pitchBendSemitones = *p->range;
    
    int mode = (int)(*p->mode);
    
    if (mode == 0)
        *p->out = FL(s.getPitchBendNormalized());
    else if (mode == 1)
        *p->out = FL(s.getPitchBendSemitones());
    else if (mode == 2)
        *p->out = FL(s.getPitchBendRatio());
    else
        *p->out = FL(s.curFreq * s.getPitchBendRatio() - s.curFreq);
    
    return OK;
}

static int monopitchbend_perfKr(CSOUND *csound, MONOPITCHBEND *p)
{
    MonoMidiState &s = MonoMidiState::get();
    int mode = (int)(*p->mode);
    
    if (mode == 0)
        *p->out = FL(s.getPitchBendNormalized());
    else if (mode == 1)
        *p->out = FL(s.getPitchBendSemitones());
    else if (mode == 2)
        *p->out = FL(s.getPitchBendRatio());
    else
        *p->out = FL(s.curFreq * s.getPitchBendRatio() - s.curFreq);
    
    return OK;
}

/* =====================================================
   MONOAFTERTOUCH - Channel aftertouch
   ===================================================== */
static int monoaftertouch_init(CSOUND *csound, MONOAFTERTOUCH *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->out = FL(s.getAftertouchNormalized());
    return OK;
}

static int monoaftertouch_perfKr(CSOUND *csound, MONOAFTERTOUCH *p)
{
    MonoMidiState &s = MonoMidiState::get();
    *p->out = FL(s.getAftertouchNormalized());
    return OK;
}

/* =====================================================
   MONOCC - MIDI Control Change
   ===================================================== */
static int monocc_init(CSOUND *csound, MONOCC *p)
{
    MonoMidiState &s = MonoMidiState::get();
    int ccnum = (int)(*p->ccnum);
    int mode = (int)(*p->mode);
    
    if (mode == 0)
        *p->out = FL(s.getCC(ccnum));
    else
        *p->out = FL(s.getCCNormalized(ccnum));
    
    return OK;
}

static int monocc_perfKr(CSOUND *csound, MONOCC *p)
{
    MonoMidiState &s = MonoMidiState::get();
    int ccnum = (int)(*p->ccnum);
    int mode = (int)(*p->mode);
    
    if (mode == 0)
        *p->out = FL(s.getCC(ccnum));
    else
        *p->out = FL(s.getCCNormalized(ccnum));
    
    return OK;
}

/* =====================================================
   OPCODE REGISTRATION
   ===================================================== */
static OENTRY localops[] =
{
    {
        (char*)"monoampmidi",
        sizeof(MONOAMPMIDI),
        0, 3,
        (char*)"i",
        (char*)"ii",
        (SUBR)monoampmidi_init,
        (SUBR)monoampmidi_perfKr,
        (SUBR)monoampmidi_perfSr
    },
    
    {
        (char*)"monocpsmidi",
        sizeof(MONOCPSMIDI),
        0, 3,
        (char*)"k",
        (char*)"iioo",
        (SUBR)monocpsmidi_init,
        (SUBR)monocpsmidi_perfKr,
        NULL
    },
    
    {
        (char*)"monoAdsr",
        sizeof(MONOADSR),
        0, 3,
        (char*)"a",
        (char*)"iiiiooo",
        (SUBR)monoAdsr_init,
        (SUBR)monoAdsr_perfKr,
        (SUBR)monoAdsr_perfSr
    },
    
    {
        (char*)"monoAdsr",
        sizeof(MONOADSR),
        0, 3,
        (char*)"k",
        (char*)"iiiiooo",
        (SUBR)monoAdsr_init,
        (SUBR)monoAdsr_perfKr,
        (SUBR)monoAdsr_perfSr
    },
    
    {
        (char*)"mononotnum",
        sizeof(MONONOTNUM),
        0, 3,
        (char*)"k",
        (char*)"",
        (SUBR)mononotnum_init,
        (SUBR)mononotnum_perfKr,
        NULL
    },
    
    {
        (char*)"mononotvel",
        sizeof(MONONOTVEL),
        0, 3,
        (char*)"k",
        (char*)"",
        (SUBR)mononotvel_init,
        (SUBR)mononotvel_perfKr,
        NULL
    },
    
    {
        (char*)"mononoteactive",
        sizeof(MONONOTEACTIVE),
        0, 3,
        (char*)"k",
        (char*)"",
        (SUBR)mononoteactive_init,
        (SUBR)mononoteactive_perfKr,
        NULL
    },
    
    {
        (char*)"monopitchbend",
        sizeof(MONOPITCHBEND),
        0, 3,
        (char*)"k",
        (char*)"oo",
        (SUBR)monopitchbend_init,
        (SUBR)monopitchbend_perfKr,
        NULL
    },
    
    {
        (char*)"monoaftertouch",
        sizeof(MONOAFTERTOUCH),
        0, 3,
        (char*)"k",
        (char*)"",
        (SUBR)monoaftertouch_init,
        (SUBR)monoaftertouch_perfKr,
        NULL
    },
    
    {
        (char*)"monocc",
        sizeof(MONOCC),
        0, 3,
        (char*)"k",
        (char*)"io",
        (SUBR)monocc_init,
        (SUBR)monocc_perfKr,
        NULL
    }
};

inline void RegisterMonoMidiOpcodes(CSOUND *csound)
{
    for (int i = 0; i < 10; i++)
    {
        csound->AppendOpcode(csound,
            localops[i].opname,
            localops[i].dsblksiz,
            localops[i].flags,
            localops[i].thread,
            localops[i].outypes,
            localops[i].intypes,
            (int (*)(CSOUND*, void*))localops[i].iopadr,
            (int (*)(CSOUND*, void*))localops[i].kopadr,
            (int (*)(CSOUND*, void*))localops[i].aopadr);
    }
}

// READMIDIDATA INSERIRE DENTRO IL PLUGINPROCESS

// int NewProjectAudioProcessor::ReadMidiData(CSOUND* /*csound*/, void* userData, unsigned char* mbuf, int nbytes)
// {
//     auto* p = static_cast<NewProjectAudioProcessor*>(userData);
//     if (!p) return 0;
    
//     int cnt = 0;
//     MidiMessage message;
//     MidiBuffer::Iterator it(p->midiBuffer);
//     int frame;
    
//     auto& state = MonoMidiState::get();
    
//     bool isMono = p->setMono.load();
    
//     // Invia note-off pendente dopo il completamento del release
//     if (isMono && state.releaseCompleted && state.lastSentNote != -1 && cnt <= nbytes - 3)
//     {
//         *mbuf++ = 0x80; // Note Off
//         *mbuf++ = (unsigned char)state.lastSentNote;
//         *mbuf++ = 0;
//         cnt += 3;
//         state.lastSentNote = -1;
//         state.releaseCompleted = false;
//     }
    
//     while (it.getNextEvent(message, frame))
//     {
//         // ===== GESTIONE PITCH BEND =====
//         if (message.isPitchWheel())
//         {
//             int pitchValue = message.getPitchWheelValue();
            
//             int lsb = pitchValue & 0x7F;
//             int msb = (pitchValue >> 7) & 0x7F;
//             state.setPitchBend(lsb, msb);
            
            
//             if (cnt <= nbytes - 3)
//             {
//                 const uint8* data = message.getRawData();
//                 *mbuf++ = data[0];
//                 *mbuf++ = data[1];
//                 *mbuf++ = 0;
//                 cnt += 3;
//             }
//             continue;

//         }
        
//         // ===== GESTIONE AFTERTOUCH =====
//         if (message.isChannelPressure())
//         {
//             state.setAftertouch(message.getChannelPressureValue());
            
//             if (cnt <= nbytes - 3)
//             {
//                 const uint8* data = message.getRawData();
//                 *mbuf++ = data[0];
//                 *mbuf++ = data[1];
//                 *mbuf++ = 0;
//                 cnt += 3;
//             }
//             continue;
//         }
        
//         // ===== GESTIONE CONTROL CHANGE =====
//         if (message.isController())
//         {
//             int ccNum = message.getControllerNumber();
//             int ccVal = message.getControllerValue();
//             state.setCC(ccNum, ccVal);
            
//             if (cnt <= nbytes - 3)
//             {
//                 const uint8* data = message.getRawData();
//                 *mbuf++ = data[0];
//                 *mbuf++ = data[1];
//                 *mbuf++ = data[2];
//                 cnt += 3;
//             }
//             continue;
//         }

//         // ===== GESTIONE NOTE ON/OFF =====
//         int note = message.getNoteNumber();
//         int vel = message.getVelocity();
        
//         if (isMono)
//         {
//             // Salva stato precedente
//             bool hadNote = state.hasActiveNote();
            
//             // Aggiorna lo state
//             if (message.isNoteOn() && vel > 0)
//                 state.noteOnEvent(note, vel);
//             else if (message.isNoteOff() || (message.isNoteOn() && vel == 0))
//                 state.noteOffEvent(note);
            
//             // Determina cosa inviare
//             bool hasNote = state.hasActiveNote();
//             int newNote = hasNote ? state.getLastNote() : -1;
            
//             // ===== MODALIT¿ MONO =====
            
//             if (hasNote && state.lastSentNote == -1)
//             {
//                 // Prima nota: invia note-on per attivare l'istanza
//                 if (cnt <= nbytes - 3)
//                 {
//                     *mbuf++ = 0x90;
//                     *mbuf++ = (unsigned char)newNote;
//                     *mbuf++ = (unsigned char)state.getLastVelocity();
//                     cnt += 3;
//                     state.lastSentNote = newNote;
//                 }
//             }
//             // Se hasNote && lastSentNote != -1: cambio nota
//             // NON inviamo nulla a Csound, il cambio Ë gestito da monocpsmidi e noteOnEvent
            
//             else if (!hasNote && hadNote)
//             {
//                 // Tutte le note rilasciate: avvia release
//                 // state.startRelease() Ë gi‡ chiamato in noteOffEvent quando stack diventa vuoto
//                 // Il note-off verr‡ inviato quando releaseCompleted diventa true
//             }
//         }
//         else
//         {
//             // ===== MODALIT¿ POLY =====
//             if (cnt <= nbytes - 3)
//             {
//                 const uint8* data = message.getRawData();
//                 *mbuf++ = data[0];
//                 *mbuf++ = data[1];
//                 *mbuf++ = data[2];
//                 cnt += 3;
//             }
//         }
//     }
    
//     p->midiBuffer.clear();
//     return cnt;
// }




#endif // CUSTOMOPCODE_H
