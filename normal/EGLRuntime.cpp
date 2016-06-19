/*
 * This confidential and proprietary software may be used only as
 * authorised by a licensing agreement from ARM Limited
 * (C) COPYRIGHT 2012 ARM Limited
 * ALL RIGHTS RESERVED
 * The entire notice above must be reproduced on all authorised
 * copies and copies may only be made to the extent permitted
 * by a licensing agreement from ARM Limited.
 */

#include "EGLRuntime.h"
#include "Platform.h"

#include <cstdlib>

namespace MaliSDK
{
    EGLDisplay EGLRuntime::display;
    EGLContext EGLRuntime::context;
    EGLSurface EGLRuntime::surface;
    EGLConfig EGLRuntime::config;

    EGLint EGLRuntime::configAttributes[] =
    {
        /* DO NOT MODIFY. */
        /* These attributes are in a known order and may be re-written at initialization according to application requests. */
        EGL_SAMPLES,             4,

        EGL_ALPHA_SIZE,          0,
//#if defined(__arm__)
        EGL_RED_SIZE,            8,
        EGL_GREEN_SIZE,          8,
        EGL_BLUE_SIZE,           8,
        EGL_BUFFER_SIZE,         32,
//#else
//        EGL_RED_SIZE,            8,
//        EGL_GREEN_SIZE,          8,
//        EGL_BLUE_SIZE,           8,
//        EGL_BUFFER_SIZE,         32,
//#endif
        EGL_STENCIL_SIZE,        0,
        EGL_RENDERABLE_TYPE,     0,    /* This field will be completed according to application request. */

        EGL_SURFACE_TYPE,        EGL_WINDOW_BIT ,

        EGL_DEPTH_SIZE,          16,

        /* MODIFY BELOW HERE. */
        /* If you need to add or override EGL attributes from above, do it below here. */

        EGL_NONE
    };

    EGLint EGLRuntime::contextAttributes[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 0, /* This field will be completed according to application request. */
        EGL_NONE, EGL_NONE,            /* If we use OpenGL ES 3.1, we will have to use EGL_CONTEXT_MINOR_VERSION,
                                          and we overwrite these fields. */
        EGL_NONE
    };

    /**
     * Using the defaults (EGL_RENDER_BUFFER = EGL_BACK_BUFFER).
     */
    EGLint EGLRuntime::windowAttributes[] =
    {
        EGL_NONE
        /*
         * Uncomment and remove EGL_NONE above to specify EGL_RENDER_BUFFER value to eglCreateWindowSurface.
         * EGL_RENDER_BUFFER,     EGL_BACK_BUFFER,
         * EGL_NONE
         */
    };

    EGLConfig EGLRuntime::findConfig(bool strictMatch)
    {
        EGLConfig *configsArray = NULL;
        EGLint numberOfConfigs = 0;
        EGLBoolean success = EGL_FALSE;

        /* Enumerate available EGL configurations which match or exceed our required attribute list. */
        success = eglChooseConfig(display, configAttributes, NULL, 0, &numberOfConfigs);
        if(success != EGL_TRUE)
        {
            EGLint error = eglGetError();
            LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
            LOGE("Failed to enumerate EGL configs at %s:%i\n", __FILE__, __LINE__);
            exit(1);
        }

        LOGD("Number of configs found is %d\n", numberOfConfigs);

        if (numberOfConfigs == 0)
        {
            LOGD("Disabling AntiAliasing to try and find a config.\n");
            configAttributes[1] = EGL_DONT_CARE;
            success = eglChooseConfig(display, configAttributes, NULL, 0, &numberOfConfigs);
            if(success != EGL_TRUE)
            {
                EGLint error = eglGetError();
                LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
                LOGE("Failed to enumerate EGL configs at %s:%i\n", __FILE__, __LINE__);
                exit(1);
            }

            if (numberOfConfigs == 0)
            {
                LOGE("No configs found with the requested attributes.\n");
                exit(1);
            }
            else
            {
                LOGD("Configs found when antialiasing disabled.\n ");
            }
        }

        /* Allocate space for all EGL configs available and get them. */
        configsArray = (EGLConfig *)calloc(numberOfConfigs, sizeof(EGLConfig));
        if(configsArray == NULL)
        {
            LOGE("Out of memory at %s:%i\n", __FILE__, __LINE__);
            exit(1);
        }
        success = eglChooseConfig(display, configAttributes, configsArray, numberOfConfigs, &numberOfConfigs);
        if(success != EGL_TRUE)
        {
            EGLint error = eglGetError();
            LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
            LOGE("Failed to enumerate EGL configs at %s:%i\n", __FILE__, __LINE__);
            exit(1);
        }

        bool matchFound = false;
        int matchingConfig = -1;

        if (strictMatch)
        {
            /*
             * Loop through the EGL configs to find a color depth match.
             * Note: This is necessary, since EGL considers a higher color depth than requested to be 'better'
             * even though this may force the driver to use a slow color conversion blitting routine. 
             */
            EGLint redSize = configAttributes[5];
            EGLint greenSize = configAttributes[7];
            EGLint blueSize = configAttributes[9];

            for(int configsIndex = 0; (configsIndex < numberOfConfigs) && !matchFound; configsIndex++)
            {
                EGLint attributeValue = 0;

                success = eglGetConfigAttrib(display, configsArray[configsIndex], EGL_RED_SIZE, &attributeValue);
                if(success != EGL_TRUE)
                {
                    EGLint error = eglGetError();
                    LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
                    LOGE("Failed to get EGL attribute at %s:%i\n", __FILE__, __LINE__);
                    exit(1);
                }

                if(attributeValue == redSize)
                {
                    success = eglGetConfigAttrib(display, configsArray[configsIndex], EGL_GREEN_SIZE, &attributeValue);
                    if(success != EGL_TRUE)
                    {
                        EGLint error = eglGetError();
                        LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
                        LOGE("Failed to get EGL attribute at %s:%i\n", __FILE__, __LINE__);
                        exit(1);
                    }

                    if(attributeValue == greenSize)
                    {
                        success = eglGetConfigAttrib(display, configsArray[configsIndex], EGL_BLUE_SIZE, &attributeValue);
                        if(success != EGL_TRUE)
                        {
                            EGLint error = eglGetError();
                            LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
                            LOGE("Failed to get EGL attribute at %s:%i\n", __FILE__, __LINE__);
                            exit(1);
                        }

                        if(attributeValue == blueSize) 
                        {
                            matchFound = true;
                            matchingConfig = configsIndex;
                        }
                    }
                }
            }
        }
        else
        {
            /* If not strict we can use any matching config. */
            matchingConfig = 0;
            matchFound = true;
        }

        if(!matchFound)
        {
            LOGE("Failed to find matching EGL config at %s:%i\n", __FILE__, __LINE__);
            exit(1);
        }

        /* Return the matching config. */
        EGLConfig configToReturn = configsArray[matchingConfig];

        free(configsArray);
        configsArray = NULL;

        return configToReturn;
    }

    void EGLRuntime::initializeEGL(OpenGLESVersion requestedAPIVersion)
    {
        Platform* platform = Platform::getInstance();

        EGLBoolean success = EGL_FALSE;

//#if defined(_WIN32)
        /* Win32 */
//        display = eglGetDisplay(platform->deviceContext);
//#elif defined(__arm__)
        /* Linux on ARM */
        display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
//#else
        /* Desktop Linux */
//        platform->display = XOpenDisplay(NULL);
//        display = eglGetDisplay(platform->display);
//#endif

        if(display == EGL_NO_DISPLAY)
        {
            EGLint error = eglGetError();
            LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
            LOGE("No EGL Display available at %s:%i\n", __FILE__, __LINE__);
            exit(1);
        }

        /* Initialize EGL. */
        success = eglInitialize(display, NULL, NULL);
        if(success != EGL_TRUE)
        {
            EGLint error = eglGetError();
            LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
            LOGE("Failed to initialize EGL at %s:%i\n", __FILE__, __LINE__);
            exit(1);
        }

        /* Depending on app-requested EGL attributes, tweak the attributes we pass to EGL. */
        if(requestedAPIVersion == OPENGLES1)
        {
            configAttributes[15] = EGL_OPENGL_ES_BIT;
            contextAttributes[1] = 1;
            contextAttributes[2] = EGL_NONE;
        }
        else if(requestedAPIVersion == OPENGLES2)
        {
            configAttributes[15] = EGL_OPENGL_ES2_BIT;
            contextAttributes[1] = 2;
            contextAttributes[2] = EGL_NONE;
        }
		/*
        else if (requestedAPIVersion == OPENGLES3)
        {
            configAttributes[15] = EGL_OPENGL_ES3_BIT_KHR;
            contextAttributes[1] = 3;
            contextAttributes[2] = EGL_NONE;
        }
        else if (requestedAPIVersion == OPENGLES31)
        {
            configAttributes[15] = EGL_OPENGL_ES3_BIT_KHR;
            contextAttributes[1] = 3;
            contextAttributes[2] = EGL_CONTEXT_MINOR_VERSION;
            contextAttributes[3] = 1;
        }
		*/
        /* 
         * Find a matching config and store it in our static variable.
         * On ARM devices perform a strict match to ensure we get the best performance.
         * On desktop devices perform a loose match to ensure greatest compatability.
         */
//#if defined(__arm__)
        config = findConfig(true);
//#else
//        config = findConfig(false);
//#endif

//#if defined(__linux__) && !defined(__arm__)
//        ((DesktopLinuxPlatform*)(platform))->createX11Window();
//#endif

        /* Create a surface. */
        surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)(platform->window), windowAttributes);
        if(surface == EGL_NO_SURFACE)
        {
            EGLint error = eglGetError();
            LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
            LOGE("Failed to create EGL surface at %s:%i\n", __FILE__, __LINE__);
            exit(1);
        }

        /* Unconditionally bind to OpenGL ES API as we exit this function, since it's the default. */
        eglBindAPI(EGL_OPENGL_ES_API);

        context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributes);
        if(context == EGL_NO_CONTEXT)
        {
            EGLint error = eglGetError();
            LOGE("eglGetError(): %i (0x%.4x)\n", (int)error, (int)error);
            LOGE("Failed to create EGL context at %s:%i\n", __FILE__, __LINE__);
            exit(1);
        }
    }

    void EGLRuntime::setEGLSamples(EGLint requiredEGLSamples)
    {
        configAttributes[1] = requiredEGLSamples;
    }

    void EGLRuntime::terminateEGL(void)
    {
        /* Shut down EGL. */
        eglBindAPI(EGL_OPENGL_ES_API);
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, context);
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroyContext(display, context);
        eglDestroySurface(display, surface);
        eglTerminate(display);
    }
}
