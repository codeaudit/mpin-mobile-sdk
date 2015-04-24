/*
 * Storage.h
 *
 *  Created on: Oct 28, 2014
 *      Author: georgi
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include "def.h"

namespace store {

class Storage: public IStorage {
	public:
		explicit Storage(jobject context, bool isMpinType);
		virtual bool SetData(const String& data);
		virtual bool GetData(String &data);
		virtual const String& GetErrorMessage() const;
		virtual ~Storage();
	private:
		// JNI CLASES ::
		jclass m_pjstorageCls;
		// JNI OBJECTS ::
		jobject m_pjstorage;
		// C++ Member variables
		String m_errorMessage;
		Storage(const Storage &);
		void setErrorMessage();
};

} /* namespace store */
#endif /* STORAGE_H_ */
