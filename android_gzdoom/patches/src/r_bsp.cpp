*** /home/draje/Code/gzdoom/src/r_bsp.cpp	2017-06-18 07:52:50.703013715 -0400
--- /home/draje/Code/GitHub/nvllsvm/GZDoom-Android/doom/src/main/jni/gzdoom21_pre/src/r_bsp.cpp	2017-06-18 09:09:24.165063945 -0400
***************
*** 809,827 ****
  {
  	if (MirrorFlags & RF_XFLIP)
  	{
! 		UoverZorg = (float)wallc->tx2 * WallTMapScale;
! 		UoverZstep = (float)(-wallc->ty2) * 32.f;
! 		InvZorg = (float)(wallc->tx2 - wallc->tx1) * WallTMapScale;
! 		InvZstep = (float)(wallc->ty1 - wallc->ty2) * 32.f;
  	}
  	else
  	{
! 		UoverZorg = (float)wallc->tx1 * WallTMapScale;
! 		UoverZstep = (float)(-wallc->ty1) * 32.f;
! 		InvZorg = (float)(wallc->tx1 - wallc->tx2) * WallTMapScale;
! 		InvZstep = (float)(wallc->ty2 - wallc->ty1) * 32.f;
  	}
- 	InitDepth();
  }
  
  void FWallTmapVals::InitFromLine(int tx1, int ty1, int tx2, int ty2)
--- 809,826 ----
  {
  	if (MirrorFlags & RF_XFLIP)
  	{
! 		UoverZorg = (float)wallc->tx2 * centerx;
! 		UoverZstep = (float)(-wallc->ty2);
! 		InvZorg = (float)(wallc->tx2 - wallc->tx1) * centerx;
! 		InvZstep = (float)(wallc->ty1 - wallc->ty2);
  	}
  	else
  	{
! 		UoverZorg = (float)wallc->tx1 * centerx;
! 		UoverZstep = (float)(-wallc->ty1);
! 		InvZorg = (float)(wallc->tx1 - wallc->tx2) * centerx;
! 		InvZstep = (float)(wallc->ty2 - wallc->ty1);
  	}
  }
  
  void FWallTmapVals::InitFromLine(int tx1, int ty1, int tx2, int ty2)
***************
*** 837,853 ****
  		fullx2 = -fullx2;
  	}
  
! 	UoverZorg = (float)fullx1 * WallTMapScale;
! 	UoverZstep = (float)(-fully1) * 32.f;
! 	InvZorg = (float)(fullx1 - fullx2) * WallTMapScale;
! 	InvZstep = (float)(fully2 - fully1) * 32.f;
! 	InitDepth();
! }
! 
! void FWallTmapVals::InitDepth()
! {
! 	DepthScale = InvZstep * WallTMapScale2;
! 	DepthOrg = -UoverZstep * WallTMapScale2;
  }
  
  //
--- 836,845 ----
  		fullx2 = -fullx2;
  	}
  
! 	UoverZorg = (float)fullx1 * centerx;
! 	UoverZstep = (float)(-fully1);
! 	InvZorg = (float)(fullx1 - fullx2) * centerx;
! 	InvZstep = (float)(fully2 - fully1);
  }
  
  //
