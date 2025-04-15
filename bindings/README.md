# Kurl

This repository contains demo projects for Kotlin/ Native libCurl interop samples.

* [Curl1](./Curl1) is based on the simple C Curl sample from https://curl.haxx.se/libcurl/c/simple.html 
* [Kurl1](./Kurl1) is a direct translation of the C sample into Kotlin with the cinterop tool

* [Curl2](./Curl2) is based on the C in-memory Curl sample from https://curl.haxx.se/libcurl/c/getinmemory.html 
* [Kurl2](./Kurl2) is an interpretation of the in-memory sample, but using Kotlin strings as a buffer

* [Kurl3](./Kurl3) uses a, re-allocing, raw memory buffer instead of a String

For further examples see this [JetBrains sample project for libCurl](https://github.com/JetBrains/kotlin-native/tree/master/samples/libcurl)