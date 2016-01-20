// //=================================================================================
// // SoundSystem.cpp
// // Author: Tyler George
// // Date  : February 10, 2015
// //=================================================================================
// 
// #include <direct.h>
// #include "ck/ck.h"
// #include "Engine/Sound/SoundSystem.hpp"
// 
// ///---------------------------------------------------------------------------------
// /// Externals
// ///---------------------------------------------------------------------------------
// SoundSystem* s_theSoundSystem = NULL;
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// SoundSystem::SoundSystem() 
// 	: m_nextSoundID( 1 )
// 	, m_workingDirectory( "" )
// {
// 	//ASSERT
// 	if( !s_theSoundSystem )
// 		s_theSoundSystem = this;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// SoundSystem::~SoundSystem()
// {
// 
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void SoundSystem::Startup()
// {
// 	static CkConfig ckConfig;
// 	CkInit( &ckConfig );
// 
// 	m_workingDirectory = _getcwd( NULL, 0 );
// 	m_workingDirectory += "/";
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void SoundSystem::Shutdown()
// {
// 	CkShutdown();
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// SoundID SoundSystem::LoadStreamingSound( const std::string& soundPath, unsigned int maxNumberOfCopies )
// {
// 	SoundID soundID = m_nextSoundID++;
// 
// 	std::string absoluteSoundPath = m_workingDirectory + soundPath;
// 	absoluteSoundPath = ConvertBackSlashesToForwardSlashes( absoluteSoundPath );
// 	Sounds sounds;
// 	for( unsigned int copyNum = 0; copyNum != maxNumberOfCopies; ++copyNum )
// 	{
// 		Sound* newSound = new Sound( absoluteSoundPath );
// 		if( newSound->IsValid() ) 
// 			sounds.push_back( newSound );
// 		
// 		else
// 		{
// 			soundID = -1;
// 			delete newSound;
// 			break;
// 		}
// 	}
// 	
// 	if( soundID != -1 ) 
// 	{
// 		bool allReady = false;
// 		while( !allReady )
// 		{
// 			allReady = true;
// 			CkUpdate();
// 			for( unsigned int soundInstanceIndex = 0; soundInstanceIndex != sounds.size(); ++soundInstanceIndex )
// 			{
// 				allReady = sounds[ soundInstanceIndex ]->IsReady();
// 				if( !allReady )
// 					break;
// 			}
// 		}
// 		m_soundMap[ soundID ] = sounds;
// 
// 	}
// 
// 	return soundID;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// Sound* SoundSystem::PlayStreamingSound( SoundID soundID, int loopCount, float volumeLevel )
// {
// 	Sound* soundPicked = NULL;
// 
// 	SoundMap::iterator soundIDIter = m_soundMap.find( soundID );
// 	if( soundIDIter != m_soundMap.end() )
// 	{
// 		Sounds& sounds = soundIDIter->second;
// 		for( Sounds::iterator soundIter = sounds.begin(); soundIter != sounds.end(); ++soundIter )
// 		{
// 			Sound* sound = ( *soundIter );
// 			if( sound->IsPlaying() )
// 				continue;
// 			if( !sound->IsReady() )
// 				continue;
// 
// 			sound->Play( loopCount, volumeLevel );
// 			soundPicked = sound;
// 			break;
// 		}
// 	}
// 	return soundPicked;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void SoundSystem::Update()
// {
// 	CkUpdate();
// 	
// 	for( SoundMap::iterator soundMapIter = m_soundMap.begin(); soundMapIter != m_soundMap.end(); ++soundMapIter )
// 	{
// 		Sounds& soundInstances = soundMapIter->second;
// 		for( Sounds::iterator soundInstanceIter = soundInstances.begin(); soundInstanceIter != soundInstances.end(); ++soundInstanceIter )
// 		{
// 			Sound* sound = ( *soundInstanceIter );
// 			if( sound && sound->NeedsReset() && !sound->IsPlaying() )
// 				sound->Reset();
// 		}
// 	}
// }