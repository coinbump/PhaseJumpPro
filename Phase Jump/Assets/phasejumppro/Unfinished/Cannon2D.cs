using UnityEngine;
using System.Collections;
using System.Collections.Generic;

namespace PJ {
	using PulseEmitMap = Dictionary<int, int>;

	/// <summary>
	/// Emitted from a cannon
	/// </summary>
	abstract class Projectile
	{
	
	}

	/// <summary>
	/// Uses raycasting for hit testing (no physics)
	/// Generally bullets move forward
	/// </summary>
	class Bullet: Projectile
	{
	}

	// TODO: Do NOT use this class. It is unfinished.
	/*
		DESIGN:
		- A bullet-hell style SHMUP can have > 200 bullets onscreen at once.
	 */
	/// <summary>
	/// Emits bullets (technically "projectiles", but that's too long.
	/// </summary>
	/// TODO: work in progress
	public class Cannon2D: Node2D
	{
		/// <summary>
		/// How are bullets distributed from the cannon.
		/// 
		/// FUTURE: support line distribution, polygons, shapes, etc. if needed.
		/// </summary>
		public enum DistributeType
		{
			Out  // Fan out from angle-center
		}

		/// <summary>
		/// Each Bullet set consists of a set of distributed bullets.
		/// </summary>
		public class BulletsInfo	// TODO: Rename this
		{
			public float angle; // Angle offset from cannon (0-1.0f in reading coordinates)
			public int count;   // Number of Bullets
			public float vary;  // Angle or space vary between Bullets
			public DistributeType distributeType;
			public float emitVelocity;
			public float emitAcceleration;
		}

		/// <summary>
		/// For each emit pulse, up to N Bullet sets can be emitted.
		/// </summary>
		public class EmitInfo
		{
			public List<BulletsInfo> bullets = new List<BulletsInfo>();
		}

		/// <summary>
		/// Internal pulse timer. Each pulse results in an emit of Bullets.
		/// </summary>
		protected class PulseTimer : PJ.PulseTimer {
			Cannon2D owner;

			public PulseTimer(Cannon2D owner) : base(AbstractTimed.Type.RunOnce) {
				this.owner = owner;
			}

			// OVERRIDE:
			public override void EvtPulse(int pulseIndex) {
				owner.EvtPulse(pulseIndex);
			}
		}
		protected PulseTimer pulseTimer;

		float emitRadius;		// Radius from center to fire.
		List<EmitInfo> emitsList = new List<EmitInfo>();			// Emit sets to emit (CHECKLIST: must map with pulseEmitMap)
		PulseEmitMap pulseEmitMap = new PulseEmitMap();	// For each pulse, link to an index in the Emit vector
		
		protected override void Start()
		{
			base.Start();

			pulseTimer = new PulseTimer(this);
		}	

		// TODO: can we do this without using angles? Use Vectors instead?
		void Emit(int index)
		{
			if (emitsList.Count == 0)
			{
				Debug.Log("WARNING. Cannon has no emit info.");
				return;
			}

			if (index >= emitsList.Count) { index = 0; }
			EmitInfo emit = emitsList[index];

			Vector2 _ca = gameObject.transform.position;

			float myAngle = RotationNormal;

			foreach (BulletsInfo p in emit.bullets) { 

				bool isEvenNum = (p.count % 2) == 0;

				for (int count = 0; count < p.count; count++)
				{
					//		Node2D obj = NewBullet(*p);
					//		//if (null == obj) { continue; }
					//		//destParent.PutNode(obj);

							Angle2 av = new Angle2();
							av.angle = myAngle;

							switch (p.distributeType)
							{
								case DistributeType.Out:
									{
										if (isEvenNum)
										{
											if (count % 2 == 0)
											{
												av.angle = myAngle - p.vary / 2.0f - p.vary * (Mathf.Floor(count / 2));
											}
											else
											{
												av.angle = myAngle + p.vary / 2.0f + p.vary * (Mathf.Floor((count - 1) / 2));
											}
										}
										else
										{
											if (count > 0)
											{
												if (count % 2 == 0)
												{
													av.angle = myAngle - p.vary * (Mathf.Floor(count / 2));
												}
												else
												{
													av.angle = myAngle + p.vary * (Mathf.Floor((count + 1) / 2));
												}
											}
										}
										break;
									}
								// FUTURE: support more distributions as needed
								default:
									break;
							}

							av.magnitude = p.emitVelocity;

					//		//2DMotion motion;
					//		//Vector2 mv = av.ToMotionVector();
					//		//motion.velocity = mv;

							av.magnitude = p.emitAcceleration;
					//		//motion.mAccel = av.ToMotionVector();

					//		//obj.mRigidbody.mMotion = motion;

							//Vector2 ca = _ca;
					//		//Vector2 fireDist = AngleUtils::DegTurnAngleHypToDistance(av.angle * 360.0f, emitRadius);
					//		//ca += fireDist;
					//		//obj.MovePositionTo(ca, false);
				}
			}
		}

		// VERIFIED:
		int PulseIndexToEmitIndex(int pulseIndex) {
			return pulseEmitMap[pulseIndex];
		}

		protected void EvtPulse(int index)
		{
			Emit(PulseIndexToEmitIndex(index));
		}

		public virtual void EvtUpdate(TimeSlice time)
		{
			pulseTimer.EvtUpdate(time);
		}
	}
}
