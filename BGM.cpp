#include "BGM.h"
#include <Novice.h>

BGM::BGM() {
	title_ = Novice::LoadAudio("./Resources/audio/title.mp3");
	stage1_ = Novice::LoadAudio("./Resources/audio/stage1.mp3");
	stage2_ = Novice::LoadAudio("./Resources/audio/stage2.mp3");
	boss_ = Novice::LoadAudio("./Resources/audio/boss.mp3");

	playTitle_ = -1;
	playStage1_ = -1;
	playStage2_ = -1;
	playBoss_ = -1;

	volume_ = 0.0f;
}

void BGM::Set(float const& volume) {
	if (Novice::IsPlayingAudio(playTitle_)) {
		Novice::SetAudioVolume(playTitle_, volume);
	}
	if (Novice::IsPlayingAudio(playStage1_)) {
		Novice::SetAudioVolume(playStage1_, volume);
	}
	if (Novice::IsPlayingAudio(playStage2_)) {
		Novice::SetAudioVolume(playStage2_, volume);
	}
	if (Novice::IsPlayingAudio(playBoss_)) {
		Novice::SetAudioVolume(playBoss_, volume);
	}
}

void BGM::Stop() {
	if (Novice::IsPlayingAudio(playTitle_)) {
		Novice::StopAudio(playTitle_);
	}
	if (Novice::IsPlayingAudio(playStage1_)) {
		Novice::StopAudio(playStage1_);
	}
	if (Novice::IsPlayingAudio(playStage2_)) {
		Novice::StopAudio(playStage2_);
	}
	if (Novice::IsPlayingAudio(playBoss_)) {
		Novice::StopAudio(playBoss_);
	}
}

void BGM::PlayBGM(int const& num) {
	switch (num) {
	case 0:
		if (!Novice::IsPlayingAudio(playTitle_)) {
			playTitle_ = Novice::PlayAudio(title_, 1, volume_);
		}
		break;
	case 1:
		if (!Novice::IsPlayingAudio(playStage1_)) {
			playStage1_ = Novice::PlayAudio(stage1_, 1, volume_);
		}
		break;
	case 2:
		if (!Novice::IsPlayingAudio(playStage2_)) {
			playStage2_ = Novice::PlayAudio(stage2_, 1, volume_);
		}
		break;
	case 3:
		if (!Novice::IsPlayingAudio(playBoss_)) {
			playBoss_ = Novice::PlayAudio(boss_, 1, volume_);
		}
		break;
	default:
		if (!Novice::IsPlayingAudio(playTitle_)) {
			playTitle_ = Novice::PlayAudio(title_, 1, volume_);
		}
		break;
	}
}
