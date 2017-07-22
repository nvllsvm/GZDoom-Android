package net.nullsum.doom


class DoomWad(val file: String = "") {
    var args: String = ""

    var selected: Boolean = false //Just used for listview

    fun getTitle(): String {
        if (file.startsWith("doom2"))
            return "DOOM 2: Hell on Earth"
        else if (file.startsWith("tnt"))
            return "Final Doom: TNT - Evilution"
        else if (file.startsWith("plutonia"))
            return "Final Doom: Plutonia Experiment"
        else if (file.startsWith("strife"))
            return "Strife"
        else if (file.startsWith("chex3"))
            return "Chex® Quest 3"
        else if (file.startsWith("chex"))
            return "Chex® Quest"
        else if (file.startsWith("harm1"))
            return "Harmony"
        else if (file.startsWith("heretic"))
            return "Heretic"
        else if (file.startsWith("hexen"))
            return "Hexen"
        else if (file.startsWith("hexdd"))
            return "Hexen: Deathkings of the Dark Citadel"
        else if (file.startsWith("hacx"))
            return "Hacx"
        else
            return "DOOM"
    }

    fun getImage(): Int {
        if (file.startsWith("doom2"))
            return R.drawable.doom2
        else if (file.startsWith("tnt"))
            return R.drawable.tnt
        else if (file.startsWith("plutonia"))
            return R.drawable.plutonia
        else if (file.startsWith("strife"))
            return R.drawable.strife
        else if (file.startsWith("chex3"))
            return R.drawable.chexquest3
        else if (file.startsWith("chex"))
            return R.drawable.chexquest
        else if (file.startsWith("harm1"))
            return R.drawable.harmony
        else if (file.startsWith("heretic"))
            return R.drawable.heretic
        else if (file.startsWith("hexen"))
            return R.drawable.hexen
        else if (file.startsWith("hexdd"))
            return R.drawable.hexendk
        else if (file.startsWith("hacx"))
            return R.drawable.hacx
        else
            return R.drawable.doom
    }
}
