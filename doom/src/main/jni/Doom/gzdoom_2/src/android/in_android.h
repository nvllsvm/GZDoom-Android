#include "beloko_common/port_act_defs.h"



#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
	TS_BLANK,
	TS_MENU,
	TS_GAME,
	TS_MAP,
	TS_CONSOLE,
	TS_CUSTOM
} touchscreemode_t;


int PortableKeyEvent(int state, int code ,int unitcode);
void PortableAction(int state, int action);

void PortableMove(float fwd, float strafe);
void PortableMoveFwd(float fwd);
void PortableMoveSide(float strafe);
void PortableLookPitch(int mode, float pitch);
void PortableLookYaw(int mode, float pitch);
void PortableCommand(const char * cmd);

void PortableAutomapControl(float zoom, float x, float y);

void PortableInit(int argc,const char ** argv);
void PortableFrame(void);

int PortableShowKeyboard(void);

touchscreemode_t PortableGetScreenMode();


#ifdef __cplusplus
}
#endif
