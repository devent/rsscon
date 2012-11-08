package com.anrisoftware.rsscon.api;

import java.io.OutputStream;

public interface NativeRssconOutputStreamFactory extends
		RssconOutputStreamFactory {

	OutputStream create(RssconNative rssconNative);
}
