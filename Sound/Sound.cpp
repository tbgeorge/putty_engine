// //=================================================================================
// // Sound.cpp
// // Author: Tyler George
// // Date  : February 10, 2015
// //=================================================================================
// 
// #include "ck/sound.h"
// #include "Engine/Sound/Sound.hpp"
// 
// struct SoundImpl
// {
// 	CkSound* m_sound;
// };
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// Sound::Sound( const std::string& absoluteSoundPath ) 
// 	: m_isValid( false )
// 	, m_needsReset( false )
// 	, m_soundImpl( NULL )
// {
// 	m_soundImpl = new SoundImpl();
// 	m_soundImpl->m_sound = CkSound::newStreamSound( absoluteSoundPath.c_str(), kCkPathType_FileSystem );
// 
// 	if( m_soundImpl->m_sound )
// 		m_isValid = true;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// Sound::~Sound()
// {
// 
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool Sound::IsValid() const
// {
// 	return m_isValid;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool Sound::IsPlaying() const
// {
// 	if( m_soundImpl && m_soundImpl->m_sound )
// 		return m_soundImpl->m_sound->isPlaying();
// 	return false;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool Sound::IsReady() const
// {
// 	if( m_soundImpl && m_soundImpl->m_sound )
// 		return m_soundImpl->m_sound->isReady();
// 	return false;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool Sound::Play( int loopCount, float volumeLevel )
// {
// 	if( m_soundImpl && m_soundImpl->m_sound )
// 	{
// 		m_soundImpl->m_sound->setLoopCount( loopCount );
// 		m_soundImpl->m_sound->setVolume( volumeLevel );
// 		m_soundImpl->m_sound->play();
// 		m_needsReset = true;
// 		return true;
// 	}
// 	return false;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// bool Sound::Stop()
// {
// 	if( m_soundImpl && m_soundImpl->m_sound )
// 	{
// 		m_soundImpl->m_sound->stop();
// 		return true;
// 	}
// 	return false;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void Sound::Reset()
// {
// 	m_soundImpl->m_sound->stop();
// 	m_soundImpl->m_sound->setPlayPosition( 0 );
// 	m_needsReset = false;
// }
// 
// ///---------------------------------------------------------------------------------
// ///
// ///---------------------------------------------------------------------------------
// void Sound::SetVolume( float volumeLevel )
// {
// 	if( m_soundImpl && m_soundImpl->m_sound )
// 	{
// 		m_soundImpl->m_sound->setVolume( volumeLevel );
// 	}
// }