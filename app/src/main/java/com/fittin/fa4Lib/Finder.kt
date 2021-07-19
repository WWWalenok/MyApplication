package ru.fittin.scanleg.utils.pas

class Finder {

    external fun GetVersion(): String
    external fun FindA4(
        maxthreadcount : Int,
        img : ByteArray,
        maxX : Int,
        maxY : Int,
        imnum : Int
    ): String
    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("Fa4-native-lib")
        }
    }
}