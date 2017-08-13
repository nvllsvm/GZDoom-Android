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
}
