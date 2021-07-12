package com.fittin.fa4Lib

class Finder {

    external fun GetVersion(): String
    external fun FindA4(): String
    companion object {

        // Used to load the 'native-lib' library on application startup.
        init {
            System.loadLibrary("Fa4-native-lib")
        }
    }
}