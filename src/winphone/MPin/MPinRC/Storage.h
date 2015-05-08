#pragma once

#include <string>
#include "mpin_sdk.h"

using namespace Platform;

namespace MPinRC
{
	/// <summary>
	/// Defines a type of a Storage.
	/// </summary>
	public enum class StorageType
	{
		SECURE,
		NONSECURE
	};

	public interface class IStorage
	{
		virtual bool SetData(String^ data);
		virtual String^ GetData();
		virtual String^ GetErrorMessage();
	};

	class StorageProxy : public MPinSDK::IStorage
	{
	private:
		MPinSDK::String errorMessage;
		MPinRC::IStorage^ managedStorage;

		void UpdateErrorMessage();

	public:
		StorageProxy(MPinRC::IStorage^ storage) { this->managedStorage = storage; };

		virtual bool SetData(const MPinSDK::String& data);
		virtual bool GetData(OUT MPinSDK::String &data);
		virtual const MPinSDK::String& GetErrorMessage() const;
	};
}