/*
 * Storage.h
 *
 *  Created on: Oct 28, 2014
 *      Author: georgi.georgiev
 */

#ifndef STORAGE_H_
#define STORAGE_H_

#include "def.h"

namespace store {

class Storage: public IStorage {
	public:
		explicit Storage(bool isMpinType);
		virtual bool SetData(const String& data);
		virtual bool GetData(String &data);
		virtual const String& GetErrorMessage() const;
		virtual ~Storage();
        void Save();
	private:
		Storage(const Storage &);
        Storage();
        void readStringFromFile(const String &, OUT String &);
        void writeStringToFile(const String &, const IN String &);
        String m_errorMessage;
        String& store;
        bool m_isMpinType;
};

} /* namespace store */
#endif /* STORAGE_H_ */
