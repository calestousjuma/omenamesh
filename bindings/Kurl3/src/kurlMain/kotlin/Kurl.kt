import interop.*
import kotlinx.cinterop.*
import platform.posix.memcpy
import kotlin.system.exitProcess
import platform.posix.size_t

fun CPointer<ByteVar>.toKString(length: Int): String {
    val bytes = this.readBytes(length)
    return bytes.decodeToString()
}

class Buffer {
    var size = 0
    var capacity = 32
    var data : CPointer<ByteVar> = nativeHeap.allocArray<ByteVar>(capacity)
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

            val newSize = localBuf.size + actualSize.toInt()

            if( newSize >= localBuf.capacity ) {
                while( newSize >= localBuf.capacity )
                    localBuf.capacity = (localBuf.capacity.toDouble() * 1.3).toInt()
                var newData = nativeHeap.allocArray<ByteVar>(localBuf.capacity)

                if( localBuf.size > 0 )
                    memcpy( newData, localBuf.data, localBuf.size.convert<size_t>() )

                localBuf.data = newData
            }
            memcpy( localBuf.data + localBuf.size, contents, actualSize );
            localBuf.size = newSize
          }

          actualSize
        }
      } )


  val res = curl_easy_perform(curl)

  if(res != CURLE_OK) {
      println("curl_easy_perform() failed: ${curl_easy_strerror(res)?.toKString()}")
  }
  else {
      println( buffer.data.toKString() )
      println( "Received ${buffer.size} characters")
      println( "Capacity: ${buffer.capacity}")
  }

  curl_easy_cleanup(curl)
  bufferRef.dispose()

}
