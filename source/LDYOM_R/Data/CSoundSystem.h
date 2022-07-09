#pragma once
#include "bass.h"

namespace CLEO
{
    class CAudioStream
    {
        friend class CSoundSystem;

        CAudioStream(const CAudioStream&);

    protected:
        HSTREAM streamInternal;
        enum eStreamState
        {
            no,
            playing,
            paused,
            stopped,
        } state;
        bool OK;
        CAudioStream();

    public:
        CAudioStream(const char *src);
        virtual ~CAudioStream();

        // actions on streams
        void Play();
        void Pause(bool change_state = true);
        void Stop();
        void Resume();
        DWORD GetLength();
        DWORD GetState();
        float GetVolume();
        void SetVolume(float val);
        void Loop(bool enable);
		HSTREAM GetInternal();

        // overloadable actions
        virtual void Set3dPosition(const CVector& pos);
        virtual void Link(CPlaceable *placable = nullptr);
        virtual void Process();
    };

    class C3DAudioStream : public CAudioStream
    {
        friend class CSoundSystem;

        C3DAudioStream(const C3DAudioStream&);

    protected:
        CPlaceable	*	link;
        BASS_3DVECTOR	position;
    public:
        C3DAudioStream(const char *src);
        virtual ~C3DAudioStream();

        // overloaded actions
        virtual void Set3dPosition(const CVector& pos);
        virtual void Link(CPlaceable *placable = nullptr);
        virtual void Process();
    };
}
