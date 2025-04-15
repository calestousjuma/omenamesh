import interop.*
import kotlinx.cinterop.*
import kotlin.system.exitProcess
import platform.posix.size_t

fun CPointer<ByteVar>.toKString(length: Int): String {
    val bytes = this.readBytes(length)
    return bytes.decodeToString()
}

class Buffer {
    var data = ""
}

fun main()
{
  val curl = curl_easy_init()
  if(curl == null)
    exitProcess(-1)

  val buffer = Buffer()
  val bufferRef = StableRef.create(buffer)

  curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");
  curl_easy_setopt(curl, CURLOPT_URL, "https://curl.haxx.se/libcurl/c/getinmemory.html")
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)

  curl_easy_setopt(curl, CURLOPT_WRITEDATA, bufferRef.asCPointer())

  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, staticCFunction {
        contents : CPointer<ByteVar>?,
        size : size_t,
        nmemb : size_t,
        userp : COpaquePointer? ->

        if( contents == null )
        {
          0u
        }
        else
        {
          val actualSize = size * nmemb
          val str = contents.toKString(actualSize.toInt())

          if( userp != null ) {
            val localBuf = userp.asStableRef<Buffer>().get()
            localBuf.data += str
          }

          actualSize
        }
      } )


  val res = curl_easy_perform(curl)

  if(res != CURLE_OK) {
      println("curl_easy_perform() failed: ${curl_easy_strerror(res)?.toKString()}")
  }
  else {
      println( buffer.data )
      println( "Received ${buffer.data.length} characters")
  }

  curl_easy_cleanup(curl)
  bufferRef.dispose()

}
