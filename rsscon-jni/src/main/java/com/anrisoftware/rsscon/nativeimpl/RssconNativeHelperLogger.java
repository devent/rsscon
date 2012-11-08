package com.anrisoftware.rsscon.nativeimpl;

import java.io.ByteArrayOutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;

import com.anrisoftware.globalpom.log.AbstractLogger;

/**
 * Logging messages for {@link RssconInputStream}.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
class RssconNativeHelperLogger extends AbstractLogger {

	/**
	 * Create logger for {@link RssconInputStream}.
	 */
	RssconNativeHelperLogger() {
		super(RssconInputStream.class);
	}

	void closeRsscon(RssconNativeHelper rsscon) {
		log.trace("Close {}.", rsscon);
	}

	void openDevice(RssconNativeHelper rsscon) {
		log.trace("Open {}.", rsscon);
	}

	void writeBuffer(RssconNativeHelper rsscon, byte[] data) {
		if (log.isTraceEnabled()) {
			log.trace("Write buffer {} to {}.", toHexString(data), rsscon);
		}
	}

	private String toHexString(byte[] data) {
		return toHexString(data, data.length);
	}

	void readBuffer(RssconNativeHelper rsscon, byte[] data, int bytes) {
		if (log.isTraceEnabled()) {
			log.trace("Write buffer {} to {}.", toHexString(data, bytes),
					rsscon);
		}
	}

	private String toHexString(byte[] array, int bytes) {
		ByteArrayOutputStream out = new ByteArrayOutputStream();
		PrintWriter writer = new PrintWriter(new OutputStreamWriter(out));
		for (int i = 0; i < bytes; i++) {
			writer.printf("%2x", array[i]);
		}
		return out.toString();
	}

}
