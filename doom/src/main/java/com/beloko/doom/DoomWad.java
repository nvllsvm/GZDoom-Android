package com.beloko.doom;

public class DoomWad {
	String title;
	String directory;
	String args = "";
	int gameDLL;
	int image; 

	public boolean selected; //Just used for listview
	
	public int getGameDLL() {
		return gameDLL;
	}


	public void setGameDLL(int gameDLL) {
		this.gameDLL = gameDLL;
	}


	public int getImage() {
		return image;
	}


	public void setImage(int image) {
		this.image = image;
	}


	DoomWad(String title,String directory)
	{
		this.title = title;
		this.directory = directory;
	}


	public String getTitle() {
		return title;
	}


	public void setTitle(String title) {
		this.title = title;
	}


	public String getDirectory() {
		return directory;
	}


	public void setDirectory(String directory) {
		this.directory = directory;
	}


	public String getArgs() {
		return args;
	}


	public void setArgs(String args) {
		this.args = args;
	}
	
	static public int getGameImage(String file)
	{ 
		if (file.contains("doom2"))
			return R.drawable.doom2;
		else if (file.contains("tnt"))
			return R.drawable.tnt;
		else if (file.contains("pluto"))
			return R.drawable.plutonia;
		else if (file.contains("strife"))
			return R.drawable.strife;
		else if (file.contains("chex3"))
			return R.drawable.chexquest3;
		else if (file.contains("chex"))
			return R.drawable.chexquest;
		else if (file.contains("harm1"))
			return R.drawable.harmony;
		else if (file.contains("heretic"))
			return R.drawable.heretic;
		else if (file.contains("hexen"))
			return R.drawable.hexen;
		else if (file.contains("hexdd"))
			return R.drawable.hexendk;
		else if (file.contains("hacx"))
			return R.drawable.hacx;
		else
			return R.drawable.doom;
	}
}
