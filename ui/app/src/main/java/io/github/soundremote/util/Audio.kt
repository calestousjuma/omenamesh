package io.github.soundremote.util

import android.media.AudioFormat
import io.github.soundremote.audio.sink.MIN_PCM_BUFFER_DURATION

object Audio {
    const val SAMPLE_RATE = 48_000
    const val CHANNEL_CONFIG = AudioFormat.CHANNEL_OUT_STEREO
    const val CHANNELS = 2
    const val SAMPLE_ENCODING = AudioFormat.ENCODING_PCM_16BIT

    /** Sample size in bytes */
    const val SAMPLE_SIZE = 2

    /** Packet duration in microseconds */
    const val PACKET_DURATION = 10_000

    /** The limit on number of packets lost in a row that should be attempted to conceal */
    const val PACKET_CONCEAL_LIMIT = MIN_PCM_BUFFER_DURATION * 1_000 / PACKET_DURATION
}
