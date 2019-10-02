// Fill out your copyright notice in the Description page of Project Settings.

#include "JumpCameraShake.h"

UJumpCameraShake::UJumpCameraShake()
{
	OscillationDuration = 0.35f;
	OscillationBlendInTime = 0.05f;
	OscillationBlendOutTime = 0.15f;

	RotOscillation.Pitch.Amplitude = -5.0f;
	RotOscillation.Pitch.Frequency = 5.0f;

	
}


