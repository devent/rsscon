package com.anrisoftware.rsscon.api;

import java.io.InputStream;

public interface NativeRssconInputStreamFactory extends
		RssconInputStreamFactory {

	InputStream create(RssconNative rssconNative);
}
