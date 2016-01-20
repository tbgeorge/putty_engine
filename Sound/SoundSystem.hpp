// //=================================================================================
// // SoundSystem.hpp
// // Author: Tyler George
// // Date  : February 10, 2015
// //=================================================================================
// 
// #pragma once
// #ifndef __included_SoundSystem__
// #define __included_SoundSystem__
// 
// #include <string>
// #include <map>
// #include "Engine/Common/EngineCommon.hpp"
// #include "Engine/Sound/Sound.hpp"
// #include "Engine/Utilities/Utilities.hpp"
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// class SoundSystem
// {
// 
// public:
// 	///---------------------------------------------------------------------------------
// 	/// Constructor/Destructor
// 	///---------------------------------------------------------------------------------
// 	SoundSystem();
// 	~SoundSystem();
// 
// 	///---------------------------------------------------------------------------------
// 	/// Initialization functions
// 	///---------------------------------------------------------------------------------
// 	void Startup();
// 	void Shutdown();
// 
// 	///---------------------------------------------------------------------------------
// 	/// Update Functions
// 	///---------------------------------------------------------------------------------
// 	SoundID LoadStreamingSound( const std::string& soundPath, unsigned int maxNumberOfCopies );
// 	Sound* PlayStreamingSound( SoundID soundID, int loopCount = 0, float volumeLevel = 1.0f );
// 	void Update();
// 
// private:
// 	///---------------------------------------------------------------------------------
// 	/// Private Member variables
// 	///---------------------------------------------------------------------------------
// 	std::string m_workingDirectory;
// 	SoundID m_nextSoundID;
// 	SoundMap m_soundMap;
// };
// 
// ///---------------------------------------------------------------------------------
// /// Externals
// ///---------------------------------------------------------------------------------
// extern SoundSystem* s_theSoundSystem;
// 
// #endif