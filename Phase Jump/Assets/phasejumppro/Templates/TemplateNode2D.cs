using UnityEngine;
using System;
using PJ;

/// <summary>
/// Template for a 2D game object.
///
/// RATING: 5 stars. Simple template.
/// CODE REVIEW: 8/9/20
/// </summary>
public class TemplateNode2D : Node2D
{
	enum State
	{
		MoveRight, MoveLeft
	}

	// Use this for initialization
	protected override void Awake()
	{
		base.Awake();
	}

	protected override void Start()
	{
		base.Start();
	}

	protected override void Update()
	{
		base.Update();
	}
}
