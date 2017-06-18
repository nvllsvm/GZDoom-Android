*** doom/src/main/jni/gzdoom/src/gl/dynlights/gl_dynlight1.cpp	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/dynlights/gl_dynlight1.cpp	2017-06-18 23:34:13.200580663 -0400
***************
*** 68,73 ****
--- 68,78 ----
  	else gl_DeleteAllAttachedLights();
  }
  
+ CUSTOM_CVAR (Bool, gl_dynlight_shader, false, CVAR_ARCHIVE | CVAR_GLOBALCONFIG | CVAR_NOINITCALL)
+ {
+ 	if (self && (gl.maxuniforms < 1024 || gl.shadermodel < 4)) self = false;
+ }
+ 
  CVAR (Bool, gl_attachedlights, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
  CVAR (Bool, gl_lights_checkside, true, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
  CVAR (Float, gl_lights_intensity, 1.0f, CVAR_ARCHIVE | CVAR_GLOBALCONFIG);
***************
*** 85,91 ****
  // Sets up the parameters to render one dynamic light onto one plane
  //
  //==========================================================================
! bool gl_GetLight(Plane & p, ADynamicLight * light, bool checkside, bool forceadditive, FDynLightData &ldata)
  {
  	Vector fn, pos;
  	int i = 0;
--- 90,97 ----
  // Sets up the parameters to render one dynamic light onto one plane
  //
  //==========================================================================
! bool gl_GetLight(Plane & p, ADynamicLight * light,
! 				 int desaturation, bool checkside, bool forceadditive, FDynLightData &ldata)
  {
  	Vector fn, pos;
  	int i = 0;
***************
*** 131,136 ****
--- 137,150 ----
  		i = 1;
  	}
  
+ 	if (desaturation>0)
+ 	{
+ 		float gray=(r*77 + g*143 + b*37)/257;
+ 
+ 		r= (r*(32-desaturation)+ gray*desaturation)/32;
+ 		g= (g*(32-desaturation)+ gray*desaturation)/32;
+ 		b= (b*(32-desaturation)+ gray*desaturation)/32;
+ 	}
  	float *data = &ldata.arrays[i][ldata.arrays[i].Reserve(8)];
  	data[0] = x;
  	data[1] = z;
***************
*** 144,146 ****
--- 158,266 ----
  }
  
  
+ 
+ 
+ //==========================================================================
+ //
+ // Sets up the parameters to render one dynamic light onto one plane
+ //
+ //==========================================================================
+ bool gl_SetupLight(Plane & p, ADynamicLight * light, Vector & nearPt, Vector & up, Vector & right, 
+ 				   float & scale, int desaturation, bool checkside, bool forceadditive)
+ {
+ 	Vector fn, pos;
+ 
+     float x = FIXED2FLOAT(light->x);
+ 	float y = FIXED2FLOAT(light->y);
+ 	float z = FIXED2FLOAT(light->z);
+ 	
+ 	float dist = fabsf(p.DistToPoint(x, z, y));
+ 	float radius = (light->GetRadius() * gl_lights_size);
+ 	
+ 	if (radius <= 0.f) return false;
+ 	if (dist > radius) return false;
+ 	if (checkside && gl_lights_checkside && p.PointOnSide(x, z, y))
+ 	{
+ 		return false;
+ 	}
+ 	if (light->owned && light->target != NULL && !light->target->IsVisibleToPlayer())
+ 	{
+ 		return false;
+ 	}
+ 
+ 	scale = 1.0f / ((2.f * radius) - dist);
+ 
+ 	// project light position onto plane (find closest point on plane)
+ 
+ 
+ 	pos.Set(x,z,y);
+ 	fn=p.Normal();
+ 	fn.GetRightUp(right, up);
+ 
+ #ifdef _MSC_VER
+ 	nearPt = pos + fn * dist;
+ #else
+ 	Vector tmpVec = fn * dist;
+ 	nearPt = pos + tmpVec;
+ #endif
+ 
+ 	float cs = 1.0f - (dist / radius);
+ 	if (gl_lights_additive || light->flags4&MF4_ADDITIVE || forceadditive) cs*=0.2f;	// otherwise the light gets too strong.
+ 	float r = light->GetRed() / 255.0f * cs * gl_lights_intensity;
+ 	float g = light->GetGreen() / 255.0f * cs * gl_lights_intensity;
+ 	float b = light->GetBlue() / 255.0f * cs * gl_lights_intensity;
+ 
+ 	if (light->IsSubtractive())
+ 	{
+ 		Vector v;
+ 		
+ 		gl_RenderState.BlendEquation(GL_FUNC_REVERSE_SUBTRACT);
+ 		v.Set(r, g, b);
+ 		r = v.Length() - r;
+ 		g = v.Length() - g;
+ 		b = v.Length() - b;
+ 	}
+ 	else
+ 	{
+ 		gl_RenderState.BlendEquation(GL_FUNC_ADD);
+ 	}
+ 	if (desaturation>0)
+ 	{
+ 		float gray=(r*77 + g*143 + b*37)/257;
+ 
+ 		r= (r*(32-desaturation)+ gray*desaturation)/32;
+ 		g= (g*(32-desaturation)+ gray*desaturation)/32;
+ 		b= (b*(32-desaturation)+ gray*desaturation)/32;
+ 	}
+ 	glColor3f(r,g,b);
+ 	return true;
+ }
+ 
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
+ bool gl_SetupLightTexture()
+ {
+ 
+ 	if (GLRenderer->gllight == NULL) return false;
+ 	FMaterial * pat = FMaterial::ValidateTexture(GLRenderer->gllight);
+ 	pat->BindPatch(CM_DEFAULT, 0);
+ 	return true;
+ }
+ 
+ 
+ //==========================================================================
+ //
+ //
+ //
+ //==========================================================================
+ 
+ inline fixed_t P_AproxDistance3(fixed_t dx, fixed_t dy, fixed_t dz)
+ {
+ 	return P_AproxDistance(P_AproxDistance(dx,dy),dz);
+ }
+ 
