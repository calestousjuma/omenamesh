import interop.*
import kotlinx.cinterop.*

fun main()
{
  val curl = curl_easy_init()
  if(curl != null) {
    curl_easy_setopt(curl, CURLOPT_URL, "https://curl.haxx.se/libcurl/c/simple.html")
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L)

    val res = curl_easy_perform(curl)

    if(res != CURLE_OK)
        println("curl_easy_perform() failed: ${curl_easy_strerror(res)?.toKString()}")

    curl_easy_cleanup(curl)
  }
}
