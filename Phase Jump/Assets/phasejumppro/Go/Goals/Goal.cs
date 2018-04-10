using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

/*
 * RATING: 4 stars. Works, with Unit Tests, but could use Encode/Decode functionality
 * CODE REVIEW: 4.9.18
 */
namespace PJ
{
	/// <summary>
	/// Goals are common in games to achieve progress.
	/// EXAMPLE: complete a task N times, achieve N experience, get to the destination, etc.
	/// </summary>
	public class Goal
	{
		private HashSet<Goal> childGoals = new HashSet<Goal>();

		private bool _isComplete;    // TRUE: Goal is complete
		private Goal parent;
		public Broadcaster broadcaster = new Broadcaster();

		// CODING:
		public string key;     // To record completed goals, must be unique.

		// A. Repeat goals (repeat task N times, call Increment)
		public int repeatTarget;       // Total # of repeats for the goal to be complete
		public int repeatCount { get; private set; }       // # of times the goal was repeated

		// B. Integer progress goals (gain N XP)
		public int _intTarget;
		private int _intProgress;

		// C. Normalized progress Goals (arrive at 1.0 for normalized completion)
		private float _normalProgress;
	
		public int TargetInt {
			get {
				return _intTarget;
			}
			set {
				_intTarget = value;
				IsComplete = _intProgress >= _intTarget;
			}
		}

		public int ProgressInt {
			get {
				return _intProgress;
			}
			set {
				_intProgress = Math.Min(_intTarget, value);
				IsComplete = _intProgress >= _intTarget;
			}
		}

		public float ProgressNormal {
			get {
				return _normalProgress;
			}
			set {
				_normalProgress = value;

 				IsComplete = _normalProgress.Equals(1.0f);
			}
		}

		public bool IsComplete {
			get {
				return _isComplete;
			}
			set {
				if (_isComplete != value) {
					_isComplete = value;
					if (_isComplete) {
						broadcaster.Broadcast(new EventGoalComplete(this));
						if (null != parent) {
							parent.EvtChildGoalComplete(this);
						}
						EvtComplete();
					}
				}
			}
		}

		protected virtual void EvtComplete() {
		}

		protected virtual void EvtChildGoalComplete(Goal goal)
		{
			IsComplete = AllChildGoalsComplete();
		}

		public void AddChildGoal(Goal child)
		{
			childGoals.Add(child);
			child.parent = this;
		}

		public bool IsChild() { return parent != null; }

		public bool IsRepeatGoal() { return repeatTarget != 0; }
		public float GetProgress() {
			if (IsRepeatGoal())
			{
				return ((float)repeatCount / (float)repeatTarget);
			}
			else if (TargetInt != 0) {
				return ((float)TargetInt / (float)ProgressInt);
			}
			else {
				return ProgressNormal;
			}
		}

		public void Reset()
		{
			IsComplete = false;
			repeatCount = 0;
			ProgressInt = 0;
			ProgressNormal = 0;
		}

		public bool AllChildGoalsComplete()
		{
			bool result = true;

			foreach (Goal childGoal in childGoals) {
				if (!childGoal.IsComplete) {
					return false;
				}
			}

			return result;
		}

		public void Increment()
		{
			if (IsComplete) { return; }
			if (!IsRepeatGoal()) { return; }

			repeatCount = Math.Min(repeatTarget, repeatCount + 1);
			IsComplete = repeatCount >= repeatTarget;
		}
	}

	public class UnitTests_Goal {
		[Test]
		public void UnitTests() {
			var repeatGoal = new Goal();
			repeatGoal.repeatTarget = 2;
			repeatGoal.Increment();
			Assert.AreEqual(1, repeatGoal.repeatCount);
			repeatGoal.Increment();
			Assert.IsTrue(repeatGoal.IsComplete);

			var normalGoal = new Goal();
			normalGoal.ProgressNormal = 1.0f;
			Assert.IsTrue(normalGoal.IsComplete);

			var intGoal = new Goal();
			intGoal.TargetInt = 2;
			Assert.IsFalse(intGoal.IsComplete);
			intGoal.ProgressInt = 2;
			Assert.IsTrue(intGoal.IsComplete);
		}

		[Test]
		public void UnitTestsTree() {
			var repeatGoal = new Goal();
			repeatGoal.repeatTarget = 2;

			var normalGoal = new Goal();

			var intGoal = new Goal();
			intGoal.TargetInt = 2;

			var treeGoal = new Goal();
			treeGoal.AddChildGoal(repeatGoal);	
			treeGoal.AddChildGoal(normalGoal);	
			treeGoal.AddChildGoal(intGoal);		
			
			repeatGoal.Increment();
			Assert.AreEqual(1, repeatGoal.repeatCount);
			repeatGoal.Increment();
			Assert.IsTrue(repeatGoal.IsComplete);
			Assert.IsFalse(treeGoal.IsComplete);
			normalGoal.ProgressNormal = 1.0f;
			Assert.IsTrue(normalGoal.IsComplete);
			Assert.IsFalse(treeGoal.IsComplete);
			Assert.IsFalse(intGoal.IsComplete);
			intGoal.ProgressInt = 2;
			Assert.IsTrue(intGoal.IsComplete);
			Assert.IsTrue(treeGoal.IsComplete);
		}

	}

}

