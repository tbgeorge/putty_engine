// //=================================================================================
// // Sound.hpp
// // Author: Tyler George
// // Date  : February 10, 2015
// //=================================================================================
// 
// #pragma once
// #ifndef __included_Sound__
// #define __included_Sound__
// 
// #include <string>
// #include <vector>
// #include <map>
// #include "Engine/Common/EngineCommon.hpp"
// 
// 
// typedef int SoundID;
// struct SoundImpl;
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// class Sound
// {
// public:
// 	///---------------------------------------------------------------------------------
// 	/// Constructors/Destructors
// 	///---------------------------------------------------------------------------------
// 	Sound( const std::string& absoluteSoundPath );
// 	~Sound();
// 
// 	///---------------------------------------------------------------------------------
// 	/// Accessors/Queries
// 	///---------------------------------------------------------------------------------
// 	bool IsValid() const;
// 	bool IsPlaying() const;
// 	bool IsReady() const;
// 	bool NeedsReset() const { return m_needsReset; }
// 
// 	///---------------------------------------------------------------------------------
// 	/// Update Functions
// 	///---------------------------------------------------------------------------------
// 	bool Play( int loopCount, float volumeLevel );
// 	bool Stop();
// 	void Reset();
// 	void SetVolume( float volumeLevel );
// 
// private:
// 	///---------------------------------------------------------------------------------
// 	/// Private member variables
// 	///---------------------------------------------------------------------------------
// 	bool m_isValid;
// 	bool m_needsReset;
// 	SoundImpl* m_soundImpl;
// };
// 
// typedef std::vector< Sound* > Sounds;
// typedef std::map< SoundID, Sounds > SoundMap;
// typedef std::vector< SoundID > SoundIDs;
// 
// #endif