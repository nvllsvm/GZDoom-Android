*** /home/draje/Code/gzdoom/src/gl/utility/gl_cycler.h	2017-06-18 07:52:50.696346991 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/gl/utility/gl_cycler.h	2017-06-18 09:09:24.168397311 -0400
***************
*** 1,52 ****
! #ifndef __GL_CYCLER_H
! #define __GL_CYCLER_H
! 
! #include "farchive.h"
! 
! typedef enum
! {
!    CYCLE_Linear,
!    CYCLE_Sin,
!    CYCLE_Cos,
!    CYCLE_SawTooth,
!    CYCLE_Square
! } CycleType;
! 
! inline FArchive &operator<< (FArchive &arc, CycleType &type)
! {
! 	BYTE val = (BYTE)type;
! 	arc << val;
! 	type = (CycleType)val;
! 	return arc;
! }
! 
! 
! class FCycler
! {
! public:
!    FCycler();
!    void Update(float diff);
!    void SetParams(float start, float end, float cycle);
!    void ShouldCycle(bool sc) { m_shouldCycle = sc; }
!    void SetCycleType(CycleType ct) { m_cycleType = ct; }
!    float GetVal() { return m_current; }
! 
!    inline operator float () const { return m_current; }
!    
!    void Serialize(FArchive & arc);
! protected:
!    float m_start, m_end, m_current;
!    float m_time, m_cycle;
!    bool m_increment, m_shouldCycle;
! 
!    CycleType m_cycleType;
! };
! 
! inline FArchive &operator<< (FArchive &arc, FCycler &type)
! {
! 	type.Serialize(arc);
! 	return arc;
! }
! 
! 
! #endif
--- 1,52 ----
! #ifndef __GL_CYCLER_H
! #define __GL_CYCLER_H
! 
! #include "farchive.h"
! 
! typedef enum
! {
!    CYCLE_Linear,
!    CYCLE_Sin,
!    CYCLE_Cos,
!    CYCLE_SawTooth,
!    CYCLE_Square
! } CycleType;
! 
! inline FArchive &operator<< (FArchive &arc, CycleType &type)
! {
! 	BYTE val = (BYTE)type;
! 	arc << val;
! 	type = (CycleType)val;
! 	return arc;
! }
! 
! 
! class FCycler
! {
! public:
!    FCycler();
!    void Update(float diff);
!    void SetParams(float start, float end, float cycle);
!    void ShouldCycle(bool sc) { m_shouldCycle = sc; }
!    void SetCycleType(CycleType ct) { m_cycleType = ct; }
!    float GetVal() { return m_current; }
! 
!    inline operator float () const { return m_current; }
!    
!    void Serialize(FArchive & arc);
! protected:
!    float m_start, m_end, m_current;
!    float m_time, m_cycle;
!    bool m_increment, m_shouldCycle;
! 
!    CycleType m_cycleType;
! };
! 
! inline FArchive &operator<< (FArchive &arc, FCycler &type)
! {
! 	type.Serialize(arc);
! 	return arc;
! }
! 
! 
! #endif
