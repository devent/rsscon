package com.anrisoftware.rsscon.api;

/**
 * Manage the reference to the native device library.
 * 
 * @author Erwin Mueller, erwin.mueller@deventm.org
 * @since 1.0
 */
public interface RssconNative {

	/**
	 * Returns the reference to the device.
	 * 
	 * @return the reference.
	 */
	long getReference();

	/**
	 * Sets the reference to the device.
	 * 
	 * @param reference
	 *            the reference.
	 */
	void setReference(long reference);
}
