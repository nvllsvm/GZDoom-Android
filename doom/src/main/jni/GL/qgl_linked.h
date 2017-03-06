#define glBlendEquation qglBlendEquation


#define glAccum qglAccum
#define glAlphaFunc qglAlphaFunc
#define glAreTexturesResident qglAreTexturesResident
#define glArrayElement qglArrayElement
#define glBegin qglBegin
#define glBindTexture qglBindTexture
#define glBitmap qglBitmap
#define glBlendFunc qglBlendFunc
#define glCallList qglCallList
#define glCallLists qglCallLists
#define glClear qglClear
#define glClearAccum qglClearAccum
#define glClearColor qglClearColor
#ifdef HAVE_GLES
#define glClearDepth qglClearDepthf
#else
#define glClearDepth qglClearDepth
#endif
#define glClearIndex qglClearIndex
#define glClearStencil qglClearStencil
#ifdef HAVE_GLES
#define glClipPlane qglClipPlanef
#else
#define glClipPlane qglClipPlane
#endif
#define glColor3b qglColor3b
#define glColor3bv qglColor3bv
#define glColor3d qglColor3d
#define glColor3dv qglColor3dv
#ifdef HAVE_GLES
#define glColor3f(r, g, b) qglColor4f(r, g, b, 1.0f)
#else
#define glColor3f qglColor3f
#endif
#define glColor3fv qglColor3fv
#define glColor3i qglColor3i
#define glColor3iv qglColor3iv
#define glColor3s qglColor3s
#define glColor3sv qglColor3sv
#define glColor3ub qglColor3ub
#define glColor3ubv qglColor3ubv
#define glColor3ui qglColor3ui
#define glColor3uiv qglColor3uiv
#define glColor3us qglColor3us
#define glColor3usv qglColor3usv
#define glColor4b qglColor4b
#define glColor4bv qglColor4bv
#define glColor4d qglColor4d
#define glColor4dv qglColor4dv
#define glColor4f qglColor4f
#define glColor4fv qglColor4fv
#define glColor4i qglColor4i
#define glColor4iv qglColor4iv
#define glColor4s qglColor4s
#define glColor4sv qglColor4sv
#define glColor4ub qglColor4ub
#ifdef HAVE_GLES
#define glColor4ubv(a) qglColor4ub((a)[0], (a)[1], (a)[2], (a)[3])
#else
#define glColor4ubv qglColor4ubv
#endif
#define glColor4ui qglColor4ui
#define glColor4uiv qglColor4uiv
#define glColor4us qglColor4us
#define glColor4usv qglColor4usv
#define glColorMask qglColorMask
#define glColorMaterial qglColorMaterial
#define glColorPointer qglColorPointer
#define glCopyPixels qglCopyPixels
#define glCopyTexImage1D qglCopyTexImage1D
#define glCopyTexImage2D qglCopyTexImage2D
#define glCopyTexSubImage1D qglCopyTexSubImage1D
#define glCopyTexSubImage2D qglCopyTexSubImage2D
#define glCullFace qglCullFace
#define glDeleteLists qglDeleteLists
#define glDeleteTextures qglDeleteTextures
#define glDepthFunc qglDepthFunc
#define glDepthMask qglDepthMask
#ifdef HAVE_GLES
#define glDepthRange qglDepthRangef
#else
#define glDepthRange qglDepthRange
#endif
#define glDisable qglDisable
#define glDisableClientState qglDisableClientState
#define glDrawArrays qglDrawArrays
#define glDrawBuffer qglDrawBuffer
#define glDrawElements qglDrawElements
#define glDrawPixels qglDrawPixels
#define glEdgeFlag qglEdgeFlag
#define glEdgeFlagPointer qglEdgeFlagPointer
#define glEdgeFlagv qglEdgeFlagv
#define glEnable qglEnable
#define glEnableClientState qglEnableClientState
#define glEnd qglEnd
#define glEndList qglEndList
#define glEvalCoord1d qglEvalCoord1d
#define glEvalCoord1dv qglEvalCoord1dv
#define glEvalCoord1f qglEvalCoord1f
#define glEvalCoord1fv qglEvalCoord1fv
#define glEvalCoord2d qglEvalCoord2d
#define glEvalCoord2dv qglEvalCoord2dv
#define glEvalCoord2f qglEvalCoord2f
#define glEvalCoord2fv qglEvalCoord2fv
#define glEvalMesh1 qglEvalMesh1
#define glEvalMesh2 qglEvalMesh2
#define glEvalPoint1 qglEvalPoint1
#define glEvalPoint2 qglEvalPoint2
#define glFeedbackBuffer qglFeedbackBuffer
#define glFinish qglFinish
#define glFlush qglFlush
#define glFogf qglFogf
#define glFogfv qglFogfv
#ifdef HAVE_GLES
#define glFogi  qglFogf
#else
#define glFogi qglFogi
#endif
#define glFogiv qglFogiv
#define glFrontFace qglFrontFace
#ifdef HAVE_GLES
#define glFrustum qglFrustumf
#else
#define glFrustum qglFrustum
#endif
#define glGenLists qglGenLists
#define glGenTextures qglGenTextures
#define glGetBooleanv qglGetBooleanv
#define glGetClipPlane qglGetClipPlane
#define glGetDoublev qglGetDoublev
#define glGetError qglGetError
#define glGetFloatv qglGetFloatv
#define glGetIntegerv qglGetIntegerv
#define glGetLightfv qglGetLightfv
#define glGetLightiv qglGetLightiv
#define glGetMapdv qglGetMapdv
#define glGetMapfv qglGetMapfv
#define glGetMapiv qglGetMapiv
#define glGetMaterialfv qglGetMaterialfv
#define glGetMaterialiv qglGetMaterialiv
#define glGetPixelMapfv qglGetPixelMapfv
#define glGetPixelMapuiv qglGetPixelMapuiv
#define glGetPixelMapusv qglGetPixelMapusv
#define glGetPointerv qglGetPointerv
#define glGetPolygonStipple qglGetPolygonStipple
#define glGetString qglGetString
#define glGetTexGendv qglGetTexGendv
#define glGetTexGenfv qglGetTexGenfv
#define glGetTexGeniv qglGetTexGeniv
#define glGetTexImage qglGetTexImage
#define glGetTexLevelParameterfv qglGetTexLevelParameterfv
#define glGetTexLevelParameteriv qglGetTexLevelParameteriv
#define glGetTexParameterfv qglGetTexParameterfv
#define glGetTexParameteriv qglGetTexParameteriv
#define glHint qglHint
#define glIndexMask qglIndexMask
#define glIndexPointer qglIndexPointer
#define glIndexd qglIndexd
#define glIndexdv qglIndexdv
#define glIndexf qglIndexf
#define glIndexfv qglIndexfv
#define glIndexi qglIndexi
#define glIndexiv qglIndexiv
#define glIndexs qglIndexs
#define glIndexsv qglIndexsv
#define glIndexub qglIndexub
#define glIndexubv qglIndexubv
#define glInitNames qglInitNames
#define glInterleavedArrays qglInterleavedArrays
#define glIsEnabled qglIsEnabled
#define glIsList qglIsList
#define glIsTexture qglIsTexture
#define glLightModelf qglLightModelf
#define glLightModelfv qglLightModelfv
#define glLightModeli qglLightModeli
#define glLightModeliv qglLightModeliv
#define glLightf qglLightf
#define glLightfv qglLightfv
#define glLighti qglLighti
#define glLightiv qglLightiv
#define glLineStipple qglLineStipple
#define glLineWidth qglLineWidth
#define glListBase qglListBase
#define glLoadIdentity qglLoadIdentity
#define glLoadMatrixd qglLoadMatrixd
#define glLoadMatrixf qglLoadMatrixf
#define glLoadName qglLoadName
#define glLogicOp qglLogicOp
#define glMap1d qglMap1d
#define glMap1f qglMap1f
#define glMap2d qglMap2d
#define glMap2f qglMap2f
#define glMapGrid1d qglMapGrid1d
#define glMapGrid1f qglMapGrid1f
#define glMapGrid2d qglMapGrid2d
#define glMapGrid2f qglMapGrid2f
#define glMaterialf qglMaterialf
#define glMaterialfv qglMaterialfv
#define glMateriali qglMateriali
#define glMaterialiv qglMaterialiv
#define glMatrixMode qglMatrixMode
#define glMultMatrixd qglMultMatrixd
#define glMultMatrixf qglMultMatrixf
#define glNewList qglNewList
#define glNormal3b qglNormal3b
#define glNormal3bv qglNormal3bv
#define glNormal3d qglNormal3d
#define glNormal3dv qglNormal3dv
#define glNormal3f qglNormal3f
#define glNormal3fv qglNormal3fv
#define glNormal3i qglNormal3i
#define glNormal3iv qglNormal3iv
#define glNormal3s qglNormal3s
#define glNormal3sv qglNormal3sv
#define glNormalPointer qglNormalPointer
#ifdef HAVE_GLES
#define glOrtho qglOrthof
#else
#define glOrtho qglOrtho
#endif
#define glPassThrough qglPassThrough
#define glPixelMapfv qglPixelMapfv
#define glPixelMapuiv qglPixelMapuiv
#define glPixelMapusv qglPixelMapusv
#define glPixelStoref qglPixelStoref
#define glPixelStorei qglPixelStorei
#define glPixelTransferf qglPixelTransferf
#define glPixelTransferi qglPixelTransferi
#define glPixelZoom qglPixelZoom
#define glPointSize qglPointSize
#define glPolygonMode qglPolygonMode
#define glPolygonOffset qglPolygonOffset
#define glPolygonStipple qglPolygonStipple
#define glPopAttrib qglPopAttrib
#define glPopClientAttrib qglPopClientAttrib
#define glPopMatrix qglPopMatrix
#define glPopName qglPopName
#define glPrioritizeTextures qglPrioritizeTextures
#define glPushAttrib qglPushAttrib
#define glPushClientAttrib qglPushClientAttrib
#define glPushMatrix qglPushMatrix
#define glPushName qglPushName
#define glRasterPos2d qglRasterPos2d
#define glRasterPos2dv qglRasterPos2dv
#define glRasterPos2f qglRasterPos2f
#define glRasterPos2fv qglRasterPos2fv
#define glRasterPos2i qglRasterPos2i
#define glRasterPos2iv qglRasterPos2iv
#define glRasterPos2s qglRasterPos2s
#define glRasterPos2sv qglRasterPos2sv
#define glRasterPos3d qglRasterPos3d
#define glRasterPos3dv qglRasterPos3dv
#define glRasterPos3f qglRasterPos3f
#define glRasterPos3fv qglRasterPos3fv
#define glRasterPos3i qglRasterPos3i
#define glRasterPos3iv qglRasterPos3iv
#define glRasterPos3s qglRasterPos3s
#define glRasterPos3sv qglRasterPos3sv
#define glRasterPos4d qglRasterPos4d
#define glRasterPos4dv qglRasterPos4dv
#define glRasterPos4f qglRasterPos4f
#define glRasterPos4fv qglRasterPos4fv
#define glRasterPos4i qglRasterPos4i
#define glRasterPos4iv qglRasterPos4iv
#define glRasterPos4s qglRasterPos4s
#define glRasterPos4sv qglRasterPos4sv
#define glReadBuffer qglReadBuffer
#define glReadPixels qglReadPixels
#define glRectd qglRectd
#define glRectdv qglRectdv
#define glRectf qglRectf
#define glRectfv qglRectfv
#define glRecti qglRecti
#define glRectiv qglRectiv
#define glRects qglRects
#define glRectsv qglRectsv
#define glRenderMode qglRenderMode
#define glRotated qglRotated
#define glRotatef qglRotatef
#define glScaled qglScaled
#define glScalef qglScalef
#define glScissor qglScissor
#define glSelectBuffer qglSelectBuffer
#define glShadeModel qglShadeModel
#define glStencilFunc qglStencilFunc
#define glStencilMask qglStencilMask
#define glStencilOp qglStencilOp
#define glTexCoord1d qglTexCoord1d
#define glTexCoord1dv qglTexCoord1dv
#define glTexCoord1f qglTexCoord1f
#define glTexCoord1fv qglTexCoord1fv
#define glTexCoord1i qglTexCoord1i
#define glTexCoord1iv qglTexCoord1iv
#define glTexCoord1s qglTexCoord1s
#define glTexCoord1sv qglTexCoord1sv
#define glTexCoord2d qglTexCoord2d
#define glTexCoord2dv qglTexCoord2dv
#define glTexCoord2f qglTexCoord2f
#define glTexCoord2fv qglTexCoord2fv
#define glTexCoord2i qglTexCoord2i
#define glTexCoord2iv qglTexCoord2iv
#define glTexCoord2s qglTexCoord2s
#define glTexCoord2sv qglTexCoord2sv
#define glTexCoord3d qglTexCoord3d
#define glTexCoord3dv qglTexCoord3dv
#define glTexCoord3f qglTexCoord3f
#define glTexCoord3fv qglTexCoord3fv
#define glTexCoord3i qglTexCoord3i
#define glTexCoord3iv qglTexCoord3iv
#define glTexCoord3s qglTexCoord3s
#define glTexCoord3sv qglTexCoord3sv
#define glTexCoord4d qglTexCoord4d
#define glTexCoord4dv qglTexCoord4dv
#define glTexCoord4f qglTexCoord4f
#define glTexCoord4fv qglTexCoord4fv
#define glTexCoord4i qglTexCoord4i
#define glTexCoord4iv qglTexCoord4iv
#define glTexCoord4s qglTexCoord4s
#define glTexCoord4sv qglTexCoord4sv
#define glTexCoordPointer qglTexCoordPointer
#define	glTexEnvf qglTexEnvf
#define	glTexEnvfv qglTexEnvfv
#define	glTexEnvi qglTexEnvi
#define	glTexEnviv qglTexEnviv
#define glTexGend qglTexGend
#define glTexGendv qglTexGendv
#define glTexGenf qglTexGenf
#define glTexGenfv qglTexGenfv
#define glTexGeni qglTexGeni
#define glTexGeniv qglTexGeniv
#define glTexImage1D qglTexImage1D
#define glTexImage2D qglTexImage2D
#define glTexParameterf qglTexParameterf
#define glTexParameterfv qglTexParameterfv
#define glTexParameteri qglTexParameteri
#define glTexParameteriv qglTexParameteriv
#define glTexSubImage1D qglTexSubImage1D
#define glTexSubImage2D qglTexSubImage2D
#define glTranslated qglTranslated
#define glTranslatef qglTranslatef
#define glVertex2d qglVertex2d
#define glVertex2dv qglVertex2dv
#define glVertex2f qglVertex2f
#define glVertex2fv qglVertex2fv
#define glVertex2i qglVertex2i
#define glVertex2iv qglVertex2iv
#define glVertex2s qglVertex2s
#define glVertex2sv qglVertex2sv
#define glVertex3d qglVertex3d
#define glVertex3dv qglVertex3dv
#define glVertex3f qglVertex3f
#define glVertex3fv qglVertex3fv
#define glVertex3i qglVertex3i
#define glVertex3iv qglVertex3iv
#define glVertex3s qglVertex3s
#define glVertex3sv qglVertex3sv
#define glVertex4d qglVertex4d
#define glVertex4dv qglVertex4dv
#define glVertex4f qglVertex4f
#define glVertex4fv qglVertex4fv
#define glVertex4i qglVertex4i
#define glVertex4iv qglVertex4iv
#define glVertex4s qglVertex4s
#define glVertex4sv qglVertex4sv
#define glVertexPointer qglVertexPointer
#define glViewport qglViewport

#ifdef HAVE_GLES
#define gl_CLAMP     qgl_CLAMP_TO_EDGE
#define gl_TEXTURE0_ARB	GL_TEXTURE0
#define gl_TEXTURE1_ARB	GL_TEXTURE1
#define gl_TEXTURE2_ARB	GL_TEXTURE2
#define gl_TEXTURE3_ARB	GL_TEXTURE3
#endif


#define glMultiTexCoord2d qglMultiTexCoord2dARB
#define glMultiTexCoord2f qglMultiTexCoord2fARB
#define glMultiTexCoord2fv qglMultiTexCoord2fv
#define glActiveTexture qglActiveTextureARB


/*
//VBO
#define glBindBuffer			qglBindBuffer
#define glDeleteBuffers			qglDeleteBuffers
#define glGenBuffers			qglGenBuffers
#define glBufferData			qglBufferData
#define glBufferSubData			qglBufferSubData
#define glMapBuffer				qglMapBuffer
#define glUnmapBuffer			qglUnmapBuffer
*/



