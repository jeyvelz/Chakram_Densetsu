#pragma once
class BGM {
public:
	BGM();
	~BGM() = default;
	void Set(float const& volume);
	void Stop();
	void PlayBGM(int const& num);
private:
	int title_;
	int stage1_;
	int stage2_;
	int boss_;

	int playTitle_;
	int playStage1_;
	int playStage2_;
	int playBoss_;

	float volume_;
};