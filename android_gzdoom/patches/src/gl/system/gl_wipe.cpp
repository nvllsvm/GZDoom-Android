*** doom/src/main/jni/gzdoom/src/gl/system/gl_wipe.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/system/gl_wipe.cpp	2017-06-18 23:34:13.227247576 -0400
***************
*** 53,64 ****
  #include "gl/renderer/gl_renderer.h"
  #include "gl/renderer/gl_renderstate.h"
  #include "gl/system/gl_framebuffer.h"
- #include "gl/shaders/gl_shader.h"
  #include "gl/textures/gl_translate.h"
  #include "gl/textures/gl_material.h"
- #include "gl/textures/gl_samplers.h"
  #include "gl/utility/gl_templates.h"
- #include "gl/data/gl_vertexbuffer.h"
  
  #ifndef _WIN32
  struct POINT {
--- 53,61 ----
***************
*** 147,159 ****
  		return false;
  	}
  
! 	wipestartscreen = new FHardwareTexture(Width, Height, true);
! 	wipestartscreen->CreateTexture(NULL, Width, Height, 0, false, 0);
! 	GLRenderer->mSamplerManager->Bind(0, CLAMP_NOFILTER);
! 	GLRenderer->mSamplerManager->Bind(1, CLAMP_NONE);
  	glFinish();
! 	wipestartscreen->Bind(0, false, false);
  	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width, Height);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
--- 144,157 ----
  		return false;
  	}
  
! 	glScalef(1,1,1);
! 	wipestartscreen = new FHardwareTexture(Width, Height, false, false, false, true);
! 	wipestartscreen->CreateTexture(NULL, Width, Height, false, 0, CM_DEFAULT);
  	glFinish();
! 	wipestartscreen->Bind(0, CM_DEFAULT);
  	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width, Height);
+ 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
+ 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
***************
*** 170,180 ****
  
  void OpenGLFrameBuffer::WipeEndScreen()
  {
! 	wipeendscreen = new FHardwareTexture(Width, Height, true);
! 	wipeendscreen->CreateTexture(NULL, Width, Height, 0, false, 0);
! 	GLRenderer->mSamplerManager->Bind(0, CLAMP_NOFILTER);
! 	glFinish();
! 	wipeendscreen->Bind(0, false, false);
  	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width, Height);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
--- 168,177 ----
  
  void OpenGLFrameBuffer::WipeEndScreen()
  {
! 	wipeendscreen = new FHardwareTexture(Width, Height, false, false, false, true);
! 	wipeendscreen->CreateTexture(NULL, Width, Height, false, 0, CM_DEFAULT);
! 	glFlush();
! 	wipeendscreen->Bind(0, CM_DEFAULT);
  	glCopyTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 0, 0, Width, Height);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
***************
*** 241,247 ****
  		delete wipeendscreen;
  		wipeendscreen = NULL;
  	}
- 	FMaterial::ClearLastTexture();
  }
  
  //==========================================================================
--- 238,243 ----
***************
*** 279,311 ****
  {
  	Clock += ticks;
  
  	float ur = fb->GetWidth() / FHardwareTexture::GetTexDimension(fb->GetWidth());
  	float vb = fb->GetHeight() / FHardwareTexture::GetTexDimension(fb->GetHeight());
! 
  	gl_RenderState.SetTextureMode(TM_OPAQUE);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
! 	gl_RenderState.ResetColor();
  	gl_RenderState.Apply();
! 	fb->wipestartscreen->Bind(0, 0, false);
! 
! 	FFlatVertex *ptr;
! 	unsigned int offset, count;
! 	ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(0, 0, 0, 0, vb);
! 	ptr++;
! 	ptr->Set(0, fb->Height, 0, 0, 0);
! 	ptr++;
! 	ptr->Set(fb->Width, 0, 0, ur, vb);
! 	ptr++;
! 	ptr->Set(fb->Width, fb->Height, 0, ur, 0);
! 	ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP, &offset, &count);
! 
! 	fb->wipeendscreen->Bind(0, 0, false);
! 	gl_RenderState.SetColorAlpha(0xffffff, clamp(Clock/32.f, 0.f, 1.f));
! 	gl_RenderState.Apply();
! 	GLRenderer->mVBO->RenderArray(GL_TRIANGLE_STRIP, offset, count);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.5f);
  	gl_RenderState.SetTextureMode(TM_MODULATE);
  
  	return Clock >= 32;
--- 275,316 ----
  {
  	Clock += ticks;
  
+ #ifdef __ANDROID__
+ 	float ur = (float)fb->GetWidth() / (float)FHardwareTexture::GetTexDimension(fb->GetWidth());
+ 	float vb = (float)fb->GetHeight() / (float)FHardwareTexture::GetTexDimension(fb->GetHeight());
+ #else
  	float ur = fb->GetWidth() / FHardwareTexture::GetTexDimension(fb->GetWidth());
  	float vb = fb->GetHeight() / FHardwareTexture::GetTexDimension(fb->GetHeight());
! #endif
  	gl_RenderState.SetTextureMode(TM_OPAQUE);
! 	gl_RenderState.EnableAlphaTest(false);
  	gl_RenderState.Apply();
! 	fb->wipestartscreen->Bind(0, CM_DEFAULT);
! 	glColor4f(1.f, 1.f, 1.f, 1.f);
! 	glBegin(GL_TRIANGLE_STRIP);
! 	glTexCoord2f(0, vb);
! 	glVertex2i(0, 0);
! 	glTexCoord2f(0, 0);
! 	glVertex2i(0, fb->Height);
! 	glTexCoord2f(ur, vb);
! 	glVertex2i(fb->Width, 0);
! 	glTexCoord2f(ur, 0);
! 	glVertex2i(fb->Width, fb->Height);
! 	glEnd();
! 
! 	fb->wipeendscreen->Bind(0, CM_DEFAULT);
! 	glColor4f(1.f, 1.f, 1.f, clamp(Clock/32.f, 0.f, 1.f));
! 	glBegin(GL_TRIANGLE_STRIP);
! 	glTexCoord2f(0, vb);
! 	glVertex2i(0, 0);
! 	glTexCoord2f(0, 0);
! 	glVertex2i(0, fb->Height);
! 	glTexCoord2f(ur, vb);
! 	glVertex2i(fb->Width, 0);
! 	glTexCoord2f(ur, 0);
! 	glVertex2i(fb->Width, fb->Height);
! 	glEnd();
! 	gl_RenderState.EnableAlphaTest(true);
  	gl_RenderState.SetTextureMode(TM_MODULATE);
  
  	return Clock >= 32;
***************
*** 341,370 ****
  
  bool OpenGLFrameBuffer::Wiper_Melt::Run(int ticks, OpenGLFrameBuffer *fb)
  {
  	float ur = fb->GetWidth() / FHardwareTexture::GetTexDimension(fb->GetWidth());
  	float vb = fb->GetHeight() / FHardwareTexture::GetTexDimension(fb->GetHeight());
! 
  	// Draw the new screen on the bottom.
  	gl_RenderState.SetTextureMode(TM_OPAQUE);
- 	gl_RenderState.ResetColor();
  	gl_RenderState.Apply();
! 	fb->wipeendscreen->Bind(0, 0, false);
! 	FFlatVertex *ptr;
! 	ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(0, 0, 0, 0, vb);
! 	ptr++;
! 	ptr->Set(0, fb->Height, 0, 0, 0);
! 	ptr++;
! 	ptr->Set(fb->Width, 0, 0, ur, vb);
! 	ptr++;
! 	ptr->Set(fb->Width, fb->Height, 0, ur, 0);
! 	ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  
  	int i, dy;
  	bool done = false;
  
! 	fb->wipestartscreen->Bind(0, 0, false);
  	// Copy the old screen in vertical strips on top of the new one.
  	while (ticks--)
  	{
--- 346,378 ----
  
  bool OpenGLFrameBuffer::Wiper_Melt::Run(int ticks, OpenGLFrameBuffer *fb)
  {
+ #ifdef __ANDROID__
+ 	float ur = (float)fb->GetWidth() / (float)FHardwareTexture::GetTexDimension(fb->GetWidth());
+ 	float vb = (float)fb->GetHeight() / (float)FHardwareTexture::GetTexDimension(fb->GetHeight());
+ #else
  	float ur = fb->GetWidth() / FHardwareTexture::GetTexDimension(fb->GetWidth());
  	float vb = fb->GetHeight() / FHardwareTexture::GetTexDimension(fb->GetHeight());
! #endif
  	// Draw the new screen on the bottom.
  	gl_RenderState.SetTextureMode(TM_OPAQUE);
  	gl_RenderState.Apply();
! 	fb->wipeendscreen->Bind(0, CM_DEFAULT);
! 	glColor4f(1.f, 1.f, 1.f, 1.f);
! 	glBegin(GL_TRIANGLE_STRIP);
! 	glTexCoord2f(0, vb);
! 	glVertex2i(0, 0);
! 	glTexCoord2f(0, 0);
! 	glVertex2i(0, fb->Height);
! 	glTexCoord2f(ur, vb);
! 	glVertex2i(fb->Width, 0);
! 	glTexCoord2f(ur, 0);
! 	glVertex2i(fb->Width, fb->Height);
! 	glEnd();
  
  	int i, dy;
  	bool done = false;
  
! 	fb->wipestartscreen->Bind(0, CM_DEFAULT);
  	// Copy the old screen in vertical strips on top of the new one.
  	while (ticks--)
  	{
***************
*** 383,391 ****
  				done = false;
  			}
  			if (ticks == 0)
! 			{ 
! 				// Only draw for the final tick.
! 				// No need for optimization. Wipes won't ever be drawn with anything else.
  				RECT rect;
  				POINT dpt;
  
--- 391,397 ----
  				done = false;
  			}
  			if (ticks == 0)
! 			{ // Only draw for the final tick.
  				RECT rect;
  				POINT dpt;
  
***************
*** 401,417 ****
  					float th = (float)FHardwareTexture::GetTexDimension(fb->Height);
  					rect.bottom = fb->Height - rect.bottom;
  					rect.top = fb->Height - rect.top;
! 
! 					ptr = GLRenderer->mVBO->GetBuffer();
! 					ptr->Set(rect.left, rect.bottom, 0, rect.left / tw, rect.top / th);
! 					ptr++;
! 					ptr->Set(rect.left, rect.top, 0, rect.left / tw, rect.bottom / th);
! 					ptr++;
! 					ptr->Set(rect.right, rect.bottom, 0, rect.right / tw, rect.top / th);
! 					ptr++;
! 					ptr->Set(rect.right, rect.top, 0, rect.right / tw, rect.bottom / th);
! 					ptr++;
! 					GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP);
  				}
  			}
  		}
--- 407,423 ----
  					float th = (float)FHardwareTexture::GetTexDimension(fb->Height);
  					rect.bottom = fb->Height - rect.bottom;
  					rect.top = fb->Height - rect.top;
! 					glColor4f(1.f, 1.f, 1.f, 1.f);
! 					glBegin(GL_TRIANGLE_STRIP);
! 					glTexCoord2f(rect.left / tw, rect.top / th);
! 					glVertex2i(rect.left, rect.bottom);
! 					glTexCoord2f(rect.left / tw, rect.bottom / th);
! 					glVertex2i(rect.left, rect.top);
! 					glTexCoord2f(rect.right / tw, rect.top / th);
! 					glVertex2i(rect.right, rect.bottom);
! 					glTexCoord2f(rect.right / tw, rect.bottom / th);
! 					glVertex2i(rect.right, rect.top);
! 					glEnd();
  				}
  			}
  		}
***************
*** 471,477 ****
  	}
  
  	if (BurnTexture != NULL) delete BurnTexture;
! 	BurnTexture = new FHardwareTexture(WIDTH, HEIGHT, true);
  
  	// Update the burn texture with the new burn data
  	BYTE rgb_buffer[WIDTH*HEIGHT*4];
--- 477,483 ----
  	}
  
  	if (BurnTexture != NULL) delete BurnTexture;
! 	BurnTexture = new FHardwareTexture(WIDTH, HEIGHT, false, false, false, true);
  
  	// Update the burn texture with the new burn data
  	BYTE rgb_buffer[WIDTH*HEIGHT*4];
***************
*** 486,527 ****
  			*dest++ = MAKEARGB(s,255,255,255);
  		}
  	}
! 
  	float ur = fb->GetWidth() / FHardwareTexture::GetTexDimension(fb->GetWidth());
  	float vb = fb->GetHeight() / FHardwareTexture::GetTexDimension(fb->GetHeight());
! 
  
  	// Put the initial screen back to the buffer.
  	gl_RenderState.SetTextureMode(TM_OPAQUE);
! 	gl_RenderState.AlphaFunc(GL_GEQUAL, 0.f);
! 	gl_RenderState.ResetColor();
  	gl_RenderState.Apply();
! 	fb->wipestartscreen->Bind(0, 0, false);
! 	FFlatVertex *ptr;
! 	unsigned int offset, count;
! 	ptr = GLRenderer->mVBO->GetBuffer();
! 	ptr->Set(0, 0, 0, 0, vb);
! 	ptr++;
! 	ptr->Set(0, fb->Height, 0, 0, 0);
! 	ptr++;
! 	ptr->Set(fb->Width, 0, 0, ur, vb);
! 	ptr++;
! 	ptr->Set(fb->Width, fb->Height, 0, ur, 0);
! 	ptr++;
! 	GLRenderer->mVBO->RenderCurrent(ptr, GL_TRIANGLE_STRIP, &offset, &count);
  
  	gl_RenderState.SetTextureMode(TM_MODULATE);
! 	gl_RenderState.SetEffect(EFF_BURN);
! 	gl_RenderState.ResetColor();
! 	gl_RenderState.Apply();
  
  	// Burn the new screen on top of it.
! 	fb->wipeendscreen->Bind(0, 0, false);
  
- 	BurnTexture->CreateTexture(rgb_buffer, WIDTH, HEIGHT, 1, true, 0);
  
! 	GLRenderer->mVBO->RenderArray(GL_TRIANGLE_STRIP, offset, count);
! 	gl_RenderState.SetEffect(EFF_NONE);
  
  	// The fire may not always stabilize, so the wipe is forced to end
  	// after an arbitrary maximum time.
--- 492,569 ----
  			*dest++ = MAKEARGB(s,255,255,255);
  		}
  	}
! #ifdef __ANDROID__
! 	float ur = (float)fb->GetWidth() / (float)FHardwareTexture::GetTexDimension(fb->GetWidth());
! 	float vb = (float)fb->GetHeight() / (float)FHardwareTexture::GetTexDimension(fb->GetHeight());
! #else
  	float ur = fb->GetWidth() / FHardwareTexture::GetTexDimension(fb->GetWidth());
  	float vb = fb->GetHeight() / FHardwareTexture::GetTexDimension(fb->GetHeight());
! #endif
  
  	// Put the initial screen back to the buffer.
  	gl_RenderState.SetTextureMode(TM_OPAQUE);
! 	gl_RenderState.EnableAlphaTest(false);
  	gl_RenderState.Apply();
! 	fb->wipestartscreen->Bind(0, CM_DEFAULT);
! 	glColor4f(1.f, 1.f, 1.f, 1.f);
! 	glBegin(GL_TRIANGLE_STRIP);
! 	glTexCoord2f(0, vb);
! 	glVertex2i(0, 0);
! 	glTexCoord2f(0, 0);
! 	glVertex2i(0, fb->Height);
! 	glTexCoord2f(ur, vb);
! 	glVertex2i(fb->Width, 0);
! 	glTexCoord2f(ur, 0);
! 	glVertex2i(fb->Width, fb->Height);
! 	glEnd();
  
  	gl_RenderState.SetTextureMode(TM_MODULATE);
! 	gl_RenderState.Apply(true);
! 	glActiveTexture(GL_TEXTURE1);
! 	glEnable(GL_TEXTURE_2D);
! 
! 	// mask out the alpha channel of the wipeendscreen.
! 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE);
! 	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB, GL_REPLACE);
! 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB, GL_TEXTURE1);
! 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB, GL_SRC_COLOR);
! 	glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA, GL_REPLACE); 
! 	glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA, GL_PREVIOUS);
! 	glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA, GL_SRC_ALPHA);
! 
! 	glActiveTexture(GL_TEXTURE0);
  
  	// Burn the new screen on top of it.
! 	glColor4f(1.f, 1.f, 1.f, 1.f);
! 	fb->wipeendscreen->Bind(1, CM_DEFAULT);
! 	//BurnTexture->Bind(0, CM_DEFAULT);
! 
! 	BurnTexture->CreateTexture(rgb_buffer, WIDTH, HEIGHT, false, 0, CM_DEFAULT);
! 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
! 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
! 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
! 	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  
  
! 	glBegin(GL_TRIANGLE_STRIP);
! 	glMultiTexCoord2f(GL_TEXTURE0, 0, 0);
! 	glMultiTexCoord2f(GL_TEXTURE1, 0, vb);
! 	glVertex2i(0, 0);
! 	glMultiTexCoord2f(GL_TEXTURE0, 0, 1);
! 	glMultiTexCoord2f(GL_TEXTURE1, 0, 0);
! 	glVertex2i(0, fb->Height);
! 	glMultiTexCoord2f(GL_TEXTURE0, 1, 0);
! 	glMultiTexCoord2f(GL_TEXTURE1, ur, vb);
! 	glVertex2i(fb->Width, 0);
! 	glMultiTexCoord2f(GL_TEXTURE0, 1, 1);
! 	glMultiTexCoord2f(GL_TEXTURE1, ur, 0);
! 	glVertex2i(fb->Width, fb->Height);
! 	glEnd();
! 
! 	glActiveTexture(GL_TEXTURE1);
! 	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
! 	glDisable(GL_TEXTURE_2D);
! 	glActiveTexture(GL_TEXTURE0);
  
  	// The fire may not always stabilize, so the wipe is forced to end
  	// after an arbitrary maximum time.
