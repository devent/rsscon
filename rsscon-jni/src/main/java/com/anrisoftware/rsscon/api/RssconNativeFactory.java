package com.anrisoftware.rsscon.api;

/**
 * Factory to create the binding to the native rsscon driver library.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
public interface RssconNativeFactory {

	/**
	 * Creates the binding to the native rsscon driver library.
	 * 
	 * @param device
	 *            the device name.
	 * 
	 * @param baudRate
	 *            the {@link BaudRate}.
	 * 
	 * @return the {@link RssconNative}.
	 */
	RssconNative create(String device, BaudRate baudRate);
}
