*** doom/src/main/jni/gzdoom/src/gl/models/gl_models.h	2017-06-18 23:15:10.663307124 -0400
--- doom/src/main/jni/Doom/gzdoom_2/src/gl/models/gl_models.h	2017-06-18 23:34:13.353915413 -0400
***************
*** 29,36 ****
  
  	virtual bool Load(const char * fn, int lumpnum, const char * buffer, int length) = 0;
  	virtual int FindFrame(const char * name) = 0;
! 	virtual void RenderFrame(FTexture * skin, int frame, int frame2, double inter, int translation=0) = 0;
! 	virtual void BuildVertexBuffer(FModelVertexBuffer *buf) = 0;
  
  
  
--- 27,37 ----
  
  	virtual bool Load(const char * fn, int lumpnum, const char * buffer, int length) = 0;
  	virtual int FindFrame(const char * name) = 0;
! 	virtual void RenderFrame(FTexture * skin, int frame, int cm, int translation=0) = 0;
! 	// [BB] Added RenderFrameInterpolated
! 	virtual void RenderFrameInterpolated(FTexture * skin, int frame, int frame2, double inter, int cm, int translation=0) = 0;
! 	virtual void MakeGLData() {}
! 	virtual void CleanGLData() {}
  
  
  
***************
*** 55,61 ****
  		int             flags;
  	};
  
! 	struct DMDModelVertex
  	{
  		float           xyz[3];
  	};
--- 56,62 ----
  		int             flags;
  	};
  
! 	struct FModelVertex
  	{
  		float           xyz[3];
  	};
***************
*** 91,99 ****
  	struct ModelFrame
  	{
  		char            name[16];
! 		DMDModelVertex *vertices;
! 		DMDModelVertex *normals;
! 		unsigned int vindex;
  	};
  
  	struct DMDLoDInfo
--- 92,99 ----
  	struct ModelFrame
  	{
  		char            name[16];
! 		FModelVertex *vertices;
! 		FModelVertex *normals;
  	};
  
  	struct DMDLoDInfo
***************
*** 107,112 ****
--- 107,113 ----
  	struct DMDLoD
  	{
  		FTriangle		* triangles;
+ 		int				* glCommands;
  	};
  
  
***************
*** 119,145 ****
  	ModelFrame  *	frames;
  	DMDLoDInfo		lodInfo[MAX_LODS];
  	DMDLoD			lods[MAX_LODS];
  	bool			allowTexComp;  // Allow texture compression with this.
  
  public:
  	FDMDModel() 
  	{ 
  		loaded = false; 
  		frames = NULL;
  		skins = NULL;
! 		for (int i = 0; i < MAX_LODS; i++)
! 		{
! 			lods[i].triangles = NULL;
! 		}
  		info.numLODs = 0;
- 		texCoords = NULL;
  	}
  	virtual ~FDMDModel();
  
  	virtual bool Load(const char * fn, int lumpnum, const char * buffer, int length);
  	virtual int FindFrame(const char * name);
! 	virtual void RenderFrame(FTexture * skin, int frame, int frame2, double inter, int translation=0);
! 	virtual void BuildVertexBuffer(FModelVertexBuffer *buf);
  
  };
  
--- 120,145 ----
  	ModelFrame  *	frames;
  	DMDLoDInfo		lodInfo[MAX_LODS];
  	DMDLoD			lods[MAX_LODS];
+ 	char           *vertexUsage;   // Bitfield for each vertex.
  	bool			allowTexComp;  // Allow texture compression with this.
  
+ 	static void RenderGLCommands(void *glCommands, unsigned int numVertices,FModelVertex * vertices);
+ 
  public:
  	FDMDModel() 
  	{ 
  		loaded = false; 
  		frames = NULL;
  		skins = NULL;
! 		lods[0].glCommands = NULL;
  		info.numLODs = 0;
  	}
  	virtual ~FDMDModel();
  
  	virtual bool Load(const char * fn, int lumpnum, const char * buffer, int length);
  	virtual int FindFrame(const char * name);
! 	virtual void RenderFrame(FTexture * skin, int frame, int cm, int translation=0);
! 	virtual void RenderFrameInterpolated(FTexture * skin, int frame, int frame2, double inter, int cm, int translation=0);
  
  };
  
***************
*** 189,203 ****
  		MD3TexCoord * texcoords;
  		MD3Vertex * vertices;
  
- 		unsigned int vindex;	// contains numframes arrays of vertices
- 		unsigned int iindex;
- 
  		MD3Surface()
  		{
  			tris=NULL;
  			vertices=NULL;
  			texcoords=NULL;
- 			vindex = iindex = UINT_MAX;
  		}
  
  		~MD3Surface()
--- 189,199 ----
***************
*** 224,243 ****
  	MD3Frame * frames;
  	MD3Surface * surfaces;
  
  public:
  	FMD3Model() { }
  	virtual ~FMD3Model();
  
  	virtual bool Load(const char * fn, int lumpnum, const char * buffer, int length);
  	virtual int FindFrame(const char * name);
! 	virtual void RenderFrame(FTexture * skin, int frame, int frame2, double inter, int translation=0);
! 	virtual void BuildVertexBuffer(FModelVertexBuffer *buf);
  };
  
  struct FVoxelVertexHash
  {
  	// Returns the hash value for a key.
! 	hash_t Hash(const FModelVertex &key) 
  	{ 
  		int ix = xs_RoundToInt(key.x);		
  		int iy = xs_RoundToInt(key.y);		
--- 220,249 ----
  	MD3Frame * frames;
  	MD3Surface * surfaces;
  
+ 	void RenderTriangles(MD3Surface * surf, MD3Vertex * vert);
+ 
  public:
  	FMD3Model() { }
  	virtual ~FMD3Model();
  
  	virtual bool Load(const char * fn, int lumpnum, const char * buffer, int length);
  	virtual int FindFrame(const char * name);
! 	virtual void RenderFrame(FTexture * skin, int frame, int cm, int translation=0);
! 	virtual void RenderFrameInterpolated(FTexture * skin, int frame, int frame2, double inter, int cm, int translation=0);
! };
! 
! class FVoxelVertexBuffer;
! 
! struct FVoxelVertex
! {
! 	float x,y,z;
! 	float u,v;
  };
  
  struct FVoxelVertexHash
  {
  	// Returns the hash value for a key.
! 	hash_t Hash(const FVoxelVertex &key) 
  	{ 
  		int ix = xs_RoundToInt(key.x);		
  		int iy = xs_RoundToInt(key.y);		
***************
*** 246,252 ****
  	}
  
  	// Compares two keys, returning zero if they are the same.
! 	int Compare(const FModelVertex &left, const FModelVertex &right) 
  	{ 
  		return left.x != right.x || left.y != right.y || left.z != right.z || left.u != right.u || left.v != right.v;
  	}
--- 252,258 ----
  	}
  
  	// Compares two keys, returning zero if they are the same.
! 	int Compare(const FVoxelVertex &left, const FVoxelVertex &right) 
  	{ 
  		return left.x != right.x || left.y != right.y || left.z != right.z || left.u != right.u || left.v != right.v;
  	}
***************
*** 260,266 ****
  	}
  };
  
! typedef TMap<FModelVertex, unsigned int, FVoxelVertexHash, FIndexInit> FVoxelMap;
  
  
  class FVoxelModel : public FModel
--- 266,272 ----
  	}
  };
  
! typedef TMap<FVoxelVertex, unsigned int, FVoxelVertexHash, FIndexInit> FVoxelMap;
  
  
  class FVoxelModel : public FModel
***************
*** 268,292 ****
  protected:
  	FVoxel *mVoxel;
  	bool mOwningVoxel;	// if created through MODELDEF deleting this object must also delete the voxel object
! 	TArray<FModelVertex> mVertices;
  	TArray<unsigned int> mIndices;
  	FTexture *mPalette;
- 	unsigned int vindex;
- 	unsigned int iindex;
  	
  	void MakeSlabPolys(int x, int y, kvxslab_t *voxptr, FVoxelMap &check);
  	void AddFace(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4, BYTE color, FVoxelMap &check);
! 	unsigned int AddVertex(FModelVertex &vert, FVoxelMap &check);
  
  public:
  	FVoxelModel(FVoxel *voxel, bool owned);
  	~FVoxelModel();
  	bool Load(const char * fn, int lumpnum, const char * buffer, int length);
  	void Initialize();
  	virtual int FindFrame(const char * name);
! 	virtual void RenderFrame(FTexture * skin, int frame, int frame2, double inter, int translation=0);
  	FTexture *GetPaletteTexture() const { return mPalette; }
- 	void BuildVertexBuffer(FModelVertexBuffer *buf);
  };
  
  
--- 274,299 ----
  protected:
  	FVoxel *mVoxel;
  	bool mOwningVoxel;	// if created through MODELDEF deleting this object must also delete the voxel object
! 	TArray<FVoxelVertex> mVertices;
  	TArray<unsigned int> mIndices;
+ 	FVoxelVertexBuffer *mVBO;
  	FTexture *mPalette;
  	
  	void MakeSlabPolys(int x, int y, kvxslab_t *voxptr, FVoxelMap &check);
  	void AddFace(int x1, int y1, int z1, int x2, int y2, int z2, int x3, int y3, int z3, int x4, int y4, int z4, BYTE color, FVoxelMap &check);
! 	void AddVertex(FVoxelVertex &vert, FVoxelMap &check);
  
  public:
  	FVoxelModel(FVoxel *voxel, bool owned);
  	~FVoxelModel();
  	bool Load(const char * fn, int lumpnum, const char * buffer, int length);
  	void Initialize();
+ 	void MakeGLData();
+ 	void CleanGLData();
  	virtual int FindFrame(const char * name);
! 	virtual void RenderFrame(FTexture * skin, int frame, int cm, int translation=0);
! 	virtual void RenderFrameInterpolated(FTexture * skin, int frame, int frame2, double inter, int cm, int translation=0);
  	FTexture *GetPaletteTexture() const { return mPalette; }
  };
  
  
***************
*** 334,344 ****
  
  class GLSprite;
  
  FSpriteModelFrame * gl_FindModelFrame(const PClass * ti, int sprite, int frame, bool dropped);
  
! void gl_RenderModel(GLSprite * spr);
  // [BB] HUD weapon model rendering functions.
! void gl_RenderHUDModel(pspdef_t *psp, fixed_t ofsx, fixed_t ofsy);
  bool gl_IsHUDModelForPlayerAvailable (player_t * player);
  
  #endif
--- 341,353 ----
  
  class GLSprite;
  
+ void gl_InitModels();
  FSpriteModelFrame * gl_FindModelFrame(const PClass * ti, int sprite, int frame, bool dropped);
  
! void gl_RenderModel(GLSprite * spr, int cm);
  // [BB] HUD weapon model rendering functions.
! void gl_RenderHUDModel(pspdef_t *psp, fixed_t ofsx, fixed_t ofsy, int cm);
  bool gl_IsHUDModelForPlayerAvailable (player_t * player);
+ void gl_CleanModelData();
  
  #endif
