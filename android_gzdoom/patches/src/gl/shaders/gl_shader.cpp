*** doom/src/main/jni/gzdoom/src/gl/shaders/gl_shader.cpp	2017-06-18 23:15:10.666640488 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/shaders/gl_shader.cpp	2017-06-18 23:34:13.420582696 -0400
***************
*** 51,63 ****
  
  #include "gl/system/gl_interface.h"
  #include "gl/data/gl_data.h"
- #include "gl/data/gl_matrix.h"
  #include "gl/renderer/gl_renderer.h"
  #include "gl/renderer/gl_renderstate.h"
  #include "gl/system/gl_cvars.h"
  #include "gl/shaders/gl_shader.h"
  #include "gl/textures/gl_material.h"
! #include "gl/dynlights/gl_lightbuffer.h"
  
  //==========================================================================
  //
--- 51,72 ----
  
  #include "gl/system/gl_interface.h"
  #include "gl/data/gl_data.h"
  #include "gl/renderer/gl_renderer.h"
  #include "gl/renderer/gl_renderstate.h"
  #include "gl/system/gl_cvars.h"
  #include "gl/shaders/gl_shader.h"
  #include "gl/textures/gl_material.h"
! 
! // these will only have an effect on SM3 cards.
! // For SM4 they are always on and for SM2 always off
! CVAR(Bool, gl_warp_shader, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
! CVAR(Bool, gl_fog_shader, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
! CVAR(Bool, gl_colormap_shader, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
! CVAR(Bool, gl_brightmap_shader, false, CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
! CVAR(Bool, gl_glow_shader, true, CVAR_ARCHIVE|CVAR_GLOBALCONFIG|CVAR_NOINITCALL)
! 
! 
! extern long gl_frameMS;
  
  //==========================================================================
  //
***************
*** 70,243 ****
  	static char buffer[10000];
  	FString error;
  
! 	int i_lump = Wads.CheckNumForFullName("shaders/glsl/shaderdefs.i");
! 	if (i_lump == -1) I_Error("Unable to load 'shaders/glsl/shaderdefs.i'");
! 	FMemLump i_data = Wads.ReadLump(i_lump);
! 
! 	int vp_lump = Wads.CheckNumForFullName(vert_prog_lump);
! 	if (vp_lump == -1) I_Error("Unable to load '%s'", vert_prog_lump);
! 	FMemLump vp_data = Wads.ReadLump(vp_lump);
! 
! 	int fp_lump = Wads.CheckNumForFullName(frag_prog_lump);
! 	if (fp_lump == -1) I_Error("Unable to load '%s'", frag_prog_lump);
! 	FMemLump fp_data = Wads.ReadLump(fp_lump);
! 
! 
! 
! //
! // The following code uses GetChars on the strings to get rid of terminating 0 characters. Do not remove or the code may break!
! //
! 	unsigned int lightbuffertype = GLRenderer->mLights->GetBufferType();
! 	unsigned int lightbuffersize = GLRenderer->mLights->GetBlockSize();
! 
! 	FString vp_comb;
! 
! 	if (lightbuffertype == GL_UNIFORM_BUFFER)
! 	{
! 		vp_comb.Format("#version 130\n#extension GL_ARB_uniform_buffer_object : require\n#define NUM_UBO_LIGHTS %d\n", lightbuffersize);
! 	}
! 	else
  	{
! 		vp_comb = "#version 400 core\n#extension GL_ARB_shader_storage_buffer_object : require\n#define SHADER_STORAGE_LIGHTS\n";
! 	}
  
! 	vp_comb << defines << i_data.GetString().GetChars();
! 	FString fp_comb = vp_comb;
  
- 	vp_comb << vp_data.GetString().GetChars() << "\n";
- 	fp_comb << fp_data.GetString().GetChars() << "\n";
  
! 	if (proc_prog_lump != NULL)
! 	{
! 		if (*proc_prog_lump != '#')
  		{
! 			int pp_lump = Wads.CheckNumForFullName(proc_prog_lump);
! 			if (pp_lump == -1) I_Error("Unable to load '%s'", proc_prog_lump);
! 			FMemLump pp_data = Wads.ReadLump(pp_lump);
  
! 			if (pp_data.GetString().IndexOf("ProcessTexel") < 0)
  			{
! 				// this looks like an old custom hardware shader.
! 				// We need to replace the ProcessTexel call to make it work.
  
! 				fp_comb.Substitute("vec4 frag = ProcessTexel();", "vec4 frag = Process(vec4(1.0));");
  			}
! 			fp_comb << pp_data.GetString().GetChars();
! 			fp_comb.Substitute("gl_TexCoord[0]", "vTexCoord");	// fix old custom shaders.
! 
! 			if (pp_data.GetString().IndexOf("ProcessLight") < 0)
  			{
! 				int pl_lump = Wads.CheckNumForFullName("shaders/glsl/func_defaultlight.fp");
! 				if (pl_lump == -1) I_Error("Unable to load '%s'", "shaders/glsl/func_defaultlight.fp");
! 				FMemLump pl_data = Wads.ReadLump(pl_lump);
! 				fp_comb << "\n" << pl_data.GetString().GetChars();
  			}
  		}
- 		else
- 		{
- 			// Proc_prog_lump is not a lump name but the source itself (from generated shaders)
- 			fp_comb << proc_prog_lump + 1;
- 		}
- 	}
- 
- 	hVertProg = glCreateShader(GL_VERTEX_SHADER);
- 	hFragProg = glCreateShader(GL_FRAGMENT_SHADER);	
- 
- 
- 	int vp_size = (int)vp_comb.Len();
- 	int fp_size = (int)fp_comb.Len();
- 
- 	const char *vp_ptr = vp_comb.GetChars();
- 	const char *fp_ptr = fp_comb.GetChars();
  
! 	glShaderSource(hVertProg, 1, &vp_ptr, &vp_size);
! 	glShaderSource(hFragProg, 1, &fp_ptr, &fp_size);
  
- 	glCompileShader(hVertProg);
- 	glCompileShader(hFragProg);
  
! 	hShader = glCreateProgram();
  
! 	glAttachShader(hShader, hVertProg);
! 	glAttachShader(hShader, hFragProg);
  
! 	glBindAttribLocation(hShader, VATTR_VERTEX, "aPosition");
! 	glBindAttribLocation(hShader, VATTR_TEXCOORD, "aTexCoord");
! 	glBindAttribLocation(hShader, VATTR_COLOR, "aColor");
! 	glBindAttribLocation(hShader, VATTR_VERTEX2, "aVertex2");
  
! 	glLinkProgram(hShader);
  
! 	glGetShaderInfoLog(hVertProg, 10000, NULL, buffer);
! 	if (*buffer) 
! 	{
! 		error << "Vertex shader:\n" << buffer << "\n";
! 	}
! 	glGetShaderInfoLog(hFragProg, 10000, NULL, buffer);
! 	if (*buffer) 
! 	{
! 		error << "Fragment shader:\n" << buffer << "\n";
! 	}
  
! 	glGetProgramInfoLog(hShader, 10000, NULL, buffer);
! 	if (*buffer) 
! 	{
! 		error << "Linking:\n" << buffer << "\n";
! 	}
! 	int linked;
! 	glGetProgramiv(hShader, GL_LINK_STATUS, &linked);
! 	if (linked == 0)
! 	{
! 		// only print message if there's an error.
! 		I_Error("Init Shader '%s':\n%s\n", name, error.GetChars());
! 	}
  
  
! 	muDesaturation.Init(hShader, "uDesaturationFactor");
! 	muFogEnabled.Init(hShader, "uFogEnabled");
! 	muTextureMode.Init(hShader, "uTextureMode");
! 	muCameraPos.Init(hShader, "uCameraPos");
! 	muLightParms.Init(hShader, "uLightAttr");
! 	muColormapStart.Init(hShader, "uFixedColormapStart");
! 	muColormapRange.Init(hShader, "uFixedColormapRange");
! 	muLightIndex.Init(hShader, "uLightIndex");
! 	muFogColor.Init(hShader, "uFogColor");
! 	muDynLightColor.Init(hShader, "uDynLightColor");
! 	muObjectColor.Init(hShader, "uObjectColor");
! 	muGlowBottomColor.Init(hShader, "uGlowBottomColor");
! 	muGlowTopColor.Init(hShader, "uGlowTopColor");
! 	muGlowBottomPlane.Init(hShader, "uGlowBottomPlane");
! 	muGlowTopPlane.Init(hShader, "uGlowTopPlane");
! 	muFixedColormap.Init(hShader, "uFixedColormap");
! 	muInterpolationFactor.Init(hShader, "uInterpolationFactor");
! 	muClipHeightTop.Init(hShader, "uClipHeightTop");
! 	muClipHeightBottom.Init(hShader, "uClipHeightBottom");
! 	muAlphaThreshold.Init(hShader, "uAlphaThreshold");
! 	muTimer.Init(hShader, "timer");
  
! 	lights_index = glGetUniformLocation(hShader, "lights");
! 	fakevb_index = glGetUniformLocation(hShader, "fakeVB");
! 	projectionmatrix_index = glGetUniformLocation(hShader, "ProjectionMatrix");
! 	viewmatrix_index = glGetUniformLocation(hShader, "ViewMatrix");
! 	modelmatrix_index = glGetUniformLocation(hShader, "ModelMatrix");
! 	texturematrix_index = glGetUniformLocation(hShader, "TextureMatrix");
  
! 	int tempindex = glGetUniformBlockIndex(hShader, "LightBufferUBO");
! 	if (tempindex != -1) glUniformBlockBinding(hShader, tempindex, LIGHTBUF_BINDINGPOINT);
  
! 	glUseProgram(hShader);
  
! 	// set up other texture units (if needed by the shader)
! 	for (int i = 2; i<16; i++)
! 	{
! 		char stringbuf[20];
! 		mysnprintf(stringbuf, 20, "texture%d", i);
! 		tempindex = glGetUniformLocation(hShader, stringbuf);
! 		if (tempindex > 0) glUniform1i(tempindex, i - 1);
  	}
! 
! 	glUseProgram(0);
! 	return !!linked;
  }
  
  //==========================================================================
--- 79,200 ----
  	static char buffer[10000];
  	FString error;
  
! 	if (gl.shadermodel > 0)
  	{
! 		int vp_lump = Wads.CheckNumForFullName(vert_prog_lump);
! 		if (vp_lump == -1) I_Error("Unable to load '%s'", vert_prog_lump);
! 		FMemLump vp_data = Wads.ReadLump(vp_lump);
  
! 		int fp_lump = Wads.CheckNumForFullName(frag_prog_lump);
! 		if (fp_lump == -1) I_Error("Unable to load '%s'", frag_prog_lump);
! 		FMemLump fp_data = Wads.ReadLump(fp_lump);
  
  
! 		FString vp_comb;
! 		FString fp_comb;
! 		vp_comb = defines;
! 		if (gl.shadermodel < 4) 
  		{
! 			vp_comb << "#define NO_SM4\n";
! 		}
! 
! 		fp_comb = vp_comb;
! 		// This uses GetChars on the strings to get rid of terminating 0 characters.
! 		vp_comb << vp_data.GetString().GetChars() << "\n";
! 		fp_comb << fp_data.GetString().GetChars() << "\n";
  
! 		if (proc_prog_lump != NULL)
! 		{
! 			if (*proc_prog_lump != '#')
  			{
! 				int pp_lump = Wads.CheckNumForFullName(proc_prog_lump);
! 				if (pp_lump == -1) I_Error("Unable to load '%s'", proc_prog_lump);
! 				FMemLump pp_data = Wads.ReadLump(pp_lump);
  
! 				fp_comb << pp_data.GetString().GetChars();
  			}
! 			else 
  			{
! 				// Proc_prog_lump is not a lump name but the source itself (from generated shaders)
! 				fp_comb << proc_prog_lump+1;
  			}
  		}
  
! 		hVertProg = glCreateShader(GL_VERTEX_SHADER);
! 		hFragProg = glCreateShader(GL_FRAGMENT_SHADER);	
  
  
! 		int vp_size = (int)vp_comb.Len();
! 		int fp_size = (int)fp_comb.Len();
  
! 		const char *vp_ptr = vp_comb.GetChars();
! 		const char *fp_ptr = fp_comb.GetChars();
  
! 		glShaderSource(hVertProg, 1, &vp_ptr, &vp_size);
! 		glShaderSource(hFragProg, 1, &fp_ptr, &fp_size);
  
! 		glCompileShader(hVertProg);
! 		glCompileShader(hFragProg);
  
! 		hShader = glCreateProgram();
  
! 		glAttachShader(hShader, hVertProg);
! 		glAttachShader(hShader, hFragProg);
  
+ 		glBindAttribLocation(hShader, VATTR_GLOWDISTANCE, "glowdistance");
+ 		glBindAttribLocation(hShader, VATTR_FOGPARAMS, "fogparams");
+ 		glBindAttribLocation(hShader, VATTR_LIGHTLEVEL, "lightlevel_in"); // Korshun.
  
! 		glLinkProgram(hShader);
  
! 		glGetShaderInfoLog(hVertProg, 10000, NULL, buffer);
! 		if (*buffer) 
! 		{
! 			error << "Vertex shader:\n" << buffer << "\n";
! 		}
! 		glGetShaderInfoLog(hFragProg, 10000, NULL, buffer);
! 		if (*buffer) 
! 		{
! 			error << "Fragment shader:\n" << buffer << "\n";
! 		}
  
! 		glGetProgramInfoLog(hShader, 10000, NULL, buffer);
! 		if (*buffer) 
! 		{
! 			error << "Linking:\n" << buffer << "\n";
! 		}
! 		int linked;
! 		glGetObjectParameteriv(hShader, GL_LINK_STATUS, &linked);
! 		if (linked == 0)
! 		{
! 			// only print message if there's an error.
! 			Printf("Init Shader '%s':\n%s\n", name, error.GetChars());
! 		}
! 		timer_index = glGetUniformLocation(hShader, "timer");
! 		desaturation_index = glGetUniformLocation(hShader, "desaturation_factor");
! 		fogenabled_index = glGetUniformLocation(hShader, "fogenabled");
! 		texturemode_index = glGetUniformLocation(hShader, "texturemode");
! 		camerapos_index = glGetUniformLocation(hShader, "camerapos");
! 		lightparms_index = glGetUniformLocation(hShader, "lightparms");
! 		colormapstart_index = glGetUniformLocation(hShader, "colormapstart");
! 		colormaprange_index = glGetUniformLocation(hShader, "colormaprange");
! 		lightrange_index = glGetUniformLocation(hShader, "lightrange");
! 		fogcolor_index = glGetUniformLocation(hShader, "fogcolor");
! 		lights_index = glGetUniformLocation(hShader, "lights");
! 		dlightcolor_index = glGetUniformLocation(hShader, "dlightcolor");
! 
! 		glowbottomcolor_index = glGetUniformLocation(hShader, "bottomglowcolor");
! 		glowtopcolor_index = glGetUniformLocation(hShader, "topglowcolor");
! 		
! 		glUseProgram(hShader);
  
! 		int texture_index = glGetUniformLocation(hShader, "texture2");
! 		if (texture_index > 0) glUniform1i(texture_index, 1);
  
! 		glUseProgram(0);
! 		return !!linked;
  	}
! 	return false;
  }
  
  //==========================================================================
***************
*** 260,300 ****
  //
  //==========================================================================
  
! bool FShader::Bind()
  {
  	GLRenderer->mShaderManager->SetActiveShader(this);
  	return true;
  }
  
  //==========================================================================
  //
! // Since all shaders are REQUIRED, any error here needs to be fatal
  //
  //==========================================================================
  
! FShader *FShaderManager::Compile (const char *ShaderName, const char *ShaderPath, bool usediscard)
  {
! 	FString defines;
! 	// this can't be in the shader code due to ATI strangeness.
! 	if (gl.MaxLights() == 128) defines += "#define MAXLIGHTS128\n";
! 	if (!usediscard) defines += "#define NO_ALPHATEST\n";
  
- 	FShader *shader = NULL;
  	try
  	{
! 		shader = new FShader(ShaderName);
! 		if (!shader->Load(ShaderName, "shaders/glsl/main.vp", "shaders/glsl/main.fp", ShaderPath, defines.GetChars()))
  		{
! 			I_FatalError("Unable to load shader %s\n", ShaderName);
  		}
  	}
  	catch(CRecoverableError &err)
  	{
! 		if (shader != NULL) delete shader;
! 		shader = NULL;
! 		I_FatalError("Unable to load shader %s:\n%s\n", ShaderName, err.GetMessage());
  	}
! 	return shader;
  }
  
  //==========================================================================
--- 217,357 ----
  //
  //==========================================================================
  
! bool FShader::Bind(float Speed)
  {
  	GLRenderer->mShaderManager->SetActiveShader(this);
+ 	if (timer_index >=0 && Speed > 0.f) glUniform1f(timer_index, gl_frameMS*Speed/1000.f);
  	return true;
  }
  
  //==========================================================================
  //
! //
  //
  //==========================================================================
  
! FShaderContainer::FShaderContainer(const char *ShaderName, const char *ShaderPath)
  {
! 	const char * shaderdefines[] = {
! 		"#define NO_GLOW\n#define NO_DESATURATE\n",
! 		"#define NO_DESATURATE\n",
! 		"#define NO_GLOW\n",
! 		"\n",
! 		"#define NO_GLOW\n#define NO_DESATURATE\n#define DYNLIGHT\n",
! 		"#define NO_DESATURATE\n#define DYNLIGHT\n",
! 		"#define NO_GLOW\n#define DYNLIGHT\n",
! 		"\n#define DYNLIGHT\n",
! 		"#define NO_GLOW\n#define NO_DESATURATE\n#define SOFTLIGHT\n",
! 		"#define NO_DESATURATE\n#define SOFTLIGHT\n",
! 		"#define NO_GLOW\n#define SOFTLIGHT\n",
! 		"\n#define SOFTLIGHT\n",
! 		"#define NO_GLOW\n#define NO_DESATURATE\n#define DYNLIGHT\n#define SOFTLIGHT\n",
! 		"#define NO_DESATURATE\n#define DYNLIGHT\n#define SOFTLIGHT\n",
! 		"#define NO_GLOW\n#define DYNLIGHT\n#define SOFTLIGHT\n",
! 		"\n#define DYNLIGHT\n#define SOFTLIGHT\n"
! 	};
! 
! 	const char * shaderdesc[] = {
! 		"::default",
! 		"::glow",
! 		"::desaturate",
! 		"::glow+desaturate",
! 		"::default+dynlight",
! 		"::glow+dynlight",
! 		"::desaturate+dynlight",
! 		"::glow+desaturate+dynlight",
! 		"::softlight",
! 		"::glow+softlight",
! 		"::desaturate+softlight",
! 		"::glow+desaturate+softlight",
! 		"::default+dynlight+softlight",
! 		"::glow+dynlight+softlight",
! 		"::desaturate+dynlight+softlight",
! 		"::glow+desaturate+dynlight+softlight",
! 	};
! 
! 	FString name;
! 
! 	name << ShaderName << "::colormap";
  
  	try
  	{
! 		shader_cm = new FShader;
! 		if (!shader_cm->Load(name, "shaders/glsl/main.vp", "shaders/glsl/main_colormap.fp", ShaderPath, "#define NO_FOG\n#define NO_GLOW\n"))
  		{
! 			delete shader_cm;
! 			shader_cm = NULL;
  		}
  	}
  	catch(CRecoverableError &err)
  	{
! 		shader_cm = NULL;
! 		I_Error("Unable to load shader %s:\n%s\n", name.GetChars(), err.GetMessage());
! 	}
! 
! 	name << ShaderName << "::foglayer";
! 
! 	try
! 	{
! 		shader_fl = new FShader;
! 		if (!shader_fl->Load(name, "shaders/glsl/main.vp", "shaders/glsl/main_foglayer.fp", ShaderPath, "#define NO_GLOW\n"))
! 		{
! 			delete shader_fl;
! 			shader_fl = NULL;
! 		}
! 	}
! 	catch (CRecoverableError &err)
! 	{
! 		shader_fl = NULL;
! 		I_Error("Unable to load shader %s:\n%s\n", name.GetChars(), err.GetMessage());
! 	}
! 
! 	if (gl.shadermodel > 2)
! 	{
! 		for(int i = 0;i < NUM_SHADERS; i++)
! 		{
! 			FString name;
! 
! 			name << ShaderName << shaderdesc[i];
! 
! 			try
! 			{
! 				FString str;
! 				if ((i&4) != 0)
! 				{
! 					if (gl.maxuniforms < 1024 || gl.shadermodel != 4)
! 					{
! 						shader[i] = NULL;
! 						continue;
! 					}
! 					// this can't be in the shader code due to ATI strangeness.
! 					str = "#version 120\n#extension GL_EXT_gpu_shader4 : enable\n";
! 					if (gl.MaxLights() == 128) str += "#define MAXLIGHTS128\n";
! 				}
! 				if ((i&8) == 0)
! 				{
! 					if (gl.shadermodel != 4)
! 					{
! 						shader[i] = NULL;
! 						continue;
! 					}
! 				}
! 				str += shaderdefines[i];
! 				shader[i] = new FShader;
! 				if (!shader[i]->Load(name, "shaders/glsl/main.vp", "shaders/glsl/main.fp", ShaderPath, str.GetChars()))
! 				{
! 					delete shader[i];
! 					shader[i] = NULL;
! 				}
! 			}
! 			catch(CRecoverableError &err)
! 			{
! 				shader[i] = NULL;
! 				I_Error("Unable to load shader %s:\n%s\n", name.GetChars(), err.GetMessage());
! 			}
! 		}
  	}
! 	else memset(shader, 0, sizeof(shader));
  }
  
  //==========================================================================
***************
*** 302,313 ****
  //
  //
  //==========================================================================
  
! void FShader::ApplyMatrices(VSMatrix *proj, VSMatrix *view)
  {
! 	Bind();
! 	glUniformMatrix4fv(projectionmatrix_index, 1, false, proj->get());
! 	glUniformMatrix4fv(viewmatrix_index, 1, false, view->get());
  }
  
  
--- 359,427 ----
  //
  //
  //==========================================================================
+ FShaderContainer::~FShaderContainer()
+ {
+ 	if (shader_cm != NULL) delete shader_cm;
+ 	if (shader_fl != NULL) delete shader_fl;
+ 	for (int i = 0; i < NUM_SHADERS; i++)
+ 	{
+ 		if (shader[i] != NULL)
+ 		{
+ 			delete shader[i];
+ 			shader[i] = NULL;
+ 		}
+ 	}
+ }
  
! //==========================================================================
! //
! //
! //
! //==========================================================================
! 
! FShader *FShaderContainer::Bind(int cm, bool glowing, float Speed, bool lights)
  {
! 	FShader *sh=NULL;
! 
! 	if (cm == CM_FOGLAYER)
! 	{
! 		if (shader_fl)
! 		{
! 			shader_fl->Bind(Speed);
! 		}
! 		return shader_fl;
! 	}
! 	else if (cm >= CM_FIRSTSPECIALCOLORMAP && cm < CM_MAXCOLORMAP)
! 	{
! 		// these are never used with any kind of lighting or fog
! 		sh = shader_cm;
! 		// [BB] If there was a problem when loading the shader, sh is NULL here.
! 		if( sh )
! 		{
! 			FSpecialColormap *map = &SpecialColormaps[cm - CM_FIRSTSPECIALCOLORMAP];
! 			sh->Bind(Speed);
! 			float m[3]= {map->ColorizeEnd[0] - map->ColorizeStart[0], 
! 				map->ColorizeEnd[1] - map->ColorizeStart[1], map->ColorizeEnd[2] - map->ColorizeStart[2]};
! 
! 			glUniform3fv(sh->colormapstart_index, 1, map->ColorizeStart);
! 			glUniform3fv(sh->colormaprange_index, 1, m);
! 		}
! 	}
! 	else
! 	{
! 		bool desat = cm>=CM_DESAT1 && cm<=CM_DESAT31;
! 		sh = shader[glowing + 2*desat + 4*lights + (glset.lightmode & 8)];
! 		// [BB] If there was a problem when loading the shader, sh is NULL here.
! 		if( sh )
! 		{
! 			sh->Bind(Speed);
! 			if (desat)
! 			{
! 				glUniform1f(sh->desaturation_index, 1.f-float(cm-CM_DESAT0)/(CM_DESAT31-CM_DESAT0));
! 			}
! 		}
! 	}
! 	return sh;
  }
  
  
***************
*** 354,363 ****
  
  static const FEffectShader effectshaders[]=
  {
! 	{ "fogboundary", "shaders/glsl/main.vp", "shaders/glsl/fogboundary.fp", NULL, "#define NO_ALPHATEST\n" },
! 	{ "spheremap", "shaders/glsl/main.vp", "shaders/glsl/main.fp", "shaders/glsl/func_normal.fp", "#define SPHEREMAP\n#define NO_ALPHATEST\n" },
! 	{ "burn", "shaders/glsl/main.vp", "shaders/glsl/burn.fp", NULL, "#define SIMPLE\n#define NO_ALPHATEST\n" },
! 	{ "stencil", "shaders/glsl/main.vp", "shaders/glsl/stencil.fp", NULL, "#define SIMPLE\n#define NO_ALPHATEST\n" },
  };
  
  
--- 468,475 ----
  
  static const FEffectShader effectshaders[]=
  {
! 	{"fogboundary", "shaders/glsl/main.vp", "shaders/glsl/fogboundary.fp", NULL, "#define NO_GLOW\n"},
! 	{"spheremap", "shaders/glsl/main.vp", "shaders/glsl/main.fp", "shaders/glsl/func_normal.fp", "#define NO_GLOW\n#define NO_DESATURATE\n#define SPHEREMAP\n#define SPHEREMAP_0\n"}
  };
  
  
***************
*** 389,434 ****
  //
  //==========================================================================
  
! void FShaderManager::CompileShaders()
  {
! 	mActiveShader = NULL;
  
! 	mTextureEffects.Clear();
! 	mTextureEffectsNAT.Clear();
! 	for (int i = 0; i < MAX_EFFECTS; i++)
! 	{
! 		mEffectShaders[i] = NULL;
! 	}
  
! 	for(int i=0;defaultshaders[i].ShaderName != NULL;i++)
  	{
! 		FShader *shc = Compile(defaultshaders[i].ShaderName, defaultshaders[i].gettexelfunc, true);
! 		mTextureEffects.Push(shc);
! 		if (i <= 3)
  		{
! 			FShader *shc = Compile(defaultshaders[i].ShaderName, defaultshaders[i].gettexelfunc, false);
! 			mTextureEffectsNAT.Push(shc);
  		}
- 	}
  
! 	for(unsigned i = 0; i < usershaders.Size(); i++)
! 	{
! 		FString name = ExtractFileBase(usershaders[i]);
! 		FName sfn = name;
  
! 		FShader *shc = Compile(sfn, usershaders[i], true);
! 		mTextureEffects.Push(shc);
! 	}
  
! 	for(int i=0;i<MAX_EFFECTS;i++)
! 	{
! 		FShader *eff = new FShader(effectshaders[i].ShaderName);
! 		if (!eff->Load(effectshaders[i].ShaderName, effectshaders[i].vp, effectshaders[i].fp1,
! 						effectshaders[i].fp2, effectshaders[i].defines))
  		{
! 			delete eff;
  		}
- 		else mEffectShaders[i] = eff;
  	}
  }
  
--- 501,555 ----
  //
  //==========================================================================
  
! void FShaderManager::Recompile()
  {
! 	Clean();
! 	CompileShaders();
! }
  
! //==========================================================================
! //
! //
! //
! //==========================================================================
  
! void FShaderManager::CompileShaders()
! {
! 	mActiveShader = mEffectShaders[0] = mEffectShaders[1] = NULL;
! 	if (gl.shadermodel > 0)
  	{
! 		for(int i=0;defaultshaders[i].ShaderName != NULL;i++)
  		{
! 			FShaderContainer * shc = new FShaderContainer(defaultshaders[i].ShaderName, defaultshaders[i].gettexelfunc);
! 			mTextureEffects.Push(shc);
! 			if (gl.shadermodel <= 2) return;	// SM2 will only initialize the default shader
  		}
  
! 		for(unsigned i = 0; i < usershaders.Size(); i++)
! 		{
! 			FString name = ExtractFileBase(usershaders[i]);
! 			FName sfn = name;
  
! 			if (gl.shadermodel > 2)
! 			{
! 				FShaderContainer * shc = new FShaderContainer(sfn, usershaders[i]);
! 				mTextureEffects.Push(shc);
! 			}
! 		}
  
! 		if (gl.shadermodel > 2)
  		{
! 			for(int i=0;i<NUM_EFFECTS;i++)
! 			{
! 				FShader *eff = new FShader();
! 				if (!eff->Load(effectshaders[i].ShaderName, effectshaders[i].vp, effectshaders[i].fp1,
! 								effectshaders[i].fp2, effectshaders[i].defines))
! 				{
! 					delete eff;
! 				}
! 				else mEffectShaders[i] = eff;
! 			}
  		}
  	}
  }
  
***************
*** 440,463 ****
  
  void FShaderManager::Clean()
  {
! 	glUseProgram(0);
! 	mActiveShader = NULL;
! 
! 	for (unsigned int i = 0; i < mTextureEffectsNAT.Size(); i++)
! 	{
! 		if (mTextureEffectsNAT[i] != NULL) delete mTextureEffectsNAT[i];
! 	}
! 	for (unsigned int i = 0; i < mTextureEffects.Size(); i++)
  	{
  		if (mTextureEffects[i] != NULL) delete mTextureEffects[i];
  	}
! 	for (int i = 0; i < MAX_EFFECTS; i++)
  	{
  		if (mEffectShaders[i] != NULL) delete mEffectShaders[i];
  		mEffectShaders[i] = NULL;
  	}
  	mTextureEffects.Clear();
- 	mTextureEffectsNAT.Clear();
  }
  
  //==========================================================================
--- 561,577 ----
  
  void FShaderManager::Clean()
  {
! 	SetActiveShader(NULL);
! 	for(unsigned int i=0;i<mTextureEffects.Size();i++)
  	{
  		if (mTextureEffects[i] != NULL) delete mTextureEffects[i];
  	}
! 	for(int i=0;i<NUM_EFFECTS;i++)
  	{
  		if (mEffectShaders[i] != NULL) delete mEffectShaders[i];
  		mEffectShaders[i] = NULL;
  	}
  	mTextureEffects.Clear();
  }
  
  //==========================================================================
***************
*** 472,478 ****
  
  	for(unsigned int i=0;i<mTextureEffects.Size();i++)
  	{
! 		if (mTextureEffects[i]->mName == sfn)
  		{
  			return i;
  		}
--- 586,592 ----
  
  	for(unsigned int i=0;i<mTextureEffects.Size();i++)
  	{
! 		if (mTextureEffects[i]->Name == sfn)
  		{
  			return i;
  		}
***************
*** 488,501 ****
  
  void FShaderManager::SetActiveShader(FShader *sh)
  {
! 	if (mActiveShader != sh)
  	{
! 		glUseProgram(sh!= NULL? sh->GetHandle() : 0);
  		mActiveShader = sh;
  	}
  }
  
- 
  //==========================================================================
  //
  //
--- 602,615 ----
  
  void FShaderManager::SetActiveShader(FShader *sh)
  {
! 	// shadermodel needs to be tested here because without it UseProgram will be NULL.
! 	if (gl.shadermodel > 0 && mActiveShader != sh)
  	{
! 		glUseProgram(sh == NULL? 0 : sh->GetHandle());
  		mActiveShader = sh;
  	}
  }
  
  //==========================================================================
  //
  //
***************
*** 504,513 ****
  
  FShader *FShaderManager::BindEffect(int effect)
  {
! 	if (effect >= 0 && effect < MAX_EFFECTS && mEffectShaders[effect] != NULL)
  	{
! 		mEffectShaders[effect]->Bind();
! 		return mEffectShaders[effect];
  	}
  	return NULL;
  }
--- 618,627 ----
  
  FShader *FShaderManager::BindEffect(int effect)
  {
! 	if (effect > 0 && effect <= NUM_EFFECTS && mEffectShaders[effect-1] != NULL)
  	{
! 		mEffectShaders[effect-1]->Bind(0);
! 		return mEffectShaders[effect-1];
  	}
  	return NULL;
  }
***************
*** 515,550 ****
  
  //==========================================================================
  //
- //
- //
- //==========================================================================
- EXTERN_CVAR(Int, gl_fuzztype)
- 
- void FShaderManager::ApplyMatrices(VSMatrix *proj, VSMatrix *view)
- {
- 	for (int i = 0; i < 4; i++)
- 	{
- 		mTextureEffects[i]->ApplyMatrices(proj, view);
- 		mTextureEffectsNAT[i]->ApplyMatrices(proj, view);
- 	}
- 	mTextureEffects[4]->ApplyMatrices(proj, view);
- 	if (gl_fuzztype != 0)
- 	{
- 		mTextureEffects[4+gl_fuzztype]->ApplyMatrices(proj, view);
- 	}
- 	for (unsigned i = 12; i < mTextureEffects.Size(); i++)
- 	{
- 		mTextureEffects[i]->ApplyMatrices(proj, view);
- 	}
- 	for (int i = 0; i < MAX_EFFECTS; i++)
- 	{
- 		mEffectShaders[i]->ApplyMatrices(proj, view);
- 	}
- 	if (mActiveShader != NULL) mActiveShader->Bind();
- }
- 
- //==========================================================================
- //
  //
  //
  //==========================================================================
--- 629,634 ----
