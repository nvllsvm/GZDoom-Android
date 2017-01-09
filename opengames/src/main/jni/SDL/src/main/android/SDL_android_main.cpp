
#include "SDL_config.h"


/* Include the SDL main definition header */
#include "SDL_main.h"

/*******************************************************************************
                 Functions called by JNI
 *******************************************************************************/
#include <jni.h>

// Called before SDL_main() to initialize JNI bindings in SDL library
extern "C" void SDL_Android_Init(JNIEnv* env, jclass cls);

// Library init
extern "C" jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
	return JNI_VERSION_1_4;
}

// Start up the SDL app
extern "C" void Java_com_beloko_libsdl_SDLLib_nativeInit(JNIEnv* env, jclass cls, jobject obj,jboolean launch)
{
	/* This interface could expand with ABI negotiation, calbacks, etc. */
	SDL_Android_Init(env, cls);

	if (launch)
	{
		/* Run the application code! */
		int status;
		char *argv[2];
		argv[0] = strdup("SDL_app");
		argv[1] = NULL;
		status = SDL_main(1, argv);
	}
	/* Do not issue an exit or the whole application will terminate instead of just the SDL thread */
	//exit(status);
}

/* vi: set ts=4 sw=4 expandtab: */
