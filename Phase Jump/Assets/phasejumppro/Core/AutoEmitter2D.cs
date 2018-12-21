//using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace PJ {
	using PulseEmitMap = Dictionary<int, int>;

	/*
		DESIGN:
		- A bullet-hell style SHMUP can have > 200 emits onscreen at once.
	 */
	/// <summary>
	/// Emits emits
	/// </summary>
	public class AutoEmitter2D: PJ.MonoBehaviour
	{
		// Template object for all emits
		public GameObject template;

		// 0-1.0. So we can rotate the emitter
		[UnityEngine.Range(0, 1.0f)]
		public float emitAngle;

		// Radius of emits from emitter position
		public float emitRadius;

		// Time speed of emitter
		public float speed = 1.0f;

		/// <summary>
		/// Information for a specific emit
		/// </summary>
		[System.Serializable]
		public class Emit
		{
			// OPTIONAL: if null, use the emitter's template
			public GameObject template;

			// Key value for emit timing
			public string name;

			// How many templates to clone at emit
			public int count;

			[UnityEngine.Range(0, 360.0f)]
			public float firstDegreeAngle;

			// Angle separation between emits
			[UnityEngine.Range(0, 360.0f)]
			public float degreeSeparation;

			// Direction velocity of emits
			public float velocity = 1.0f; 

			// FUTURE: support firstOffset, distance if needed for line of emits (use Vector3 for future compatability)
		}
		public List<Emit> emits = new List<Emit>();

		[System.Serializable]
		public class EmitTiming
		{
			public string name;
			public float timeToNext = 1.0f;
		}

		// Specifies timing/order of emits
		public List<EmitTiming> timing = new List<EmitTiming>();

		protected int currentTiming;
		protected Timer timer = new Timer();

		protected virtual void Awake()
		{
			timer = new Timer();
			ResetTimer();
		}

		protected virtual void Start()
		{
		}

		Emit GetEmitByName(string emitName)
		{
			foreach (Emit emit in emits)
			{
				if (emit.name == emitName)
				{
					return emit;
				}
			}
			return null;
		}

		void EmitNext()
		{
			if (currentTiming >= timing.Count) { return; }

			EmitTiming theTiming = timing[currentTiming];
			var emitName = theTiming.name;

			if (emitName == "") { return; }
			var emit = GetEmitByName(emitName);
			if (null == emit)
			{
				Debug.Log(string.Format("WARNING. Missing emit info: {0}.", emitName));
				return;
			}

			// Emitter can have its own rotation if we'd prefer not to rotate the game object
			float degreeAngle = emit.firstDegreeAngle + emitAngle*360.0f; 

			// Add in our rotation.
			Node2D myNode2D = GetComponent<Node2D>();
			if (null != myNode2D)
			{
				degreeAngle += myNode2D.RotationDegreeAngle;
			}

			for (int i = 0; i < emit.count; i++) {
				GameObject spawn = emit.template;	// ?? operator doesn't work with Unity 
				if (null == spawn) {
					spawn =template;
				}

				var position = transform.position;
				Vector2 offset = AngleUtils.DegreeAngleToVector2(degreeAngle, emitRadius);
				position = new Vector2(position.x + offset.x, position.y + offset.y);

				GameObject go = Instantiate(spawn, position, Quaternion.identity);    // Clone it

				Node2D node2D = go.GetComponent<Node2D>();
				if (null != node2D)
				{
					node2D.RotationDegreeAngle = degreeAngle;
					node2D.velocity = AngleUtils.DegreeAngleToVector2(degreeAngle, emit.velocity);
				}
				else
				{
					Debug.Log("WARNING. AutoEmitter2D requires a Node2D for velocity.");
				}

				degreeAngle += emit.degreeSeparation;
			}
		}

		protected void ResetTimer()
		{
			if (currentTiming < timing.Count)
			{
				timer.duration = timing[currentTiming].timeToNext;
				timer.Reset();
			}
		}

		protected virtual void Update()
		{
			EvtUpdate(new TimeSlice(Time.deltaTime));
		}

		protected virtual void OnValidate()
		{
			ResetTimer();
		}

		protected virtual void EvtUpdate(TimeSlice time)
		{
			timer.EvtUpdate(time);
			if (timer.IsFinished)
			{
				EmitNext();

				currentTiming += 1;
				currentTiming %= timing.Count;
				ResetTimer();
			}
		}


#if UNITY_EDITOR
		protected override void RenderGizmos(EditorUtils.RenderState renderState)
		{
			EditorUtils.RenderCircleGizmo(transform.position, emitRadius, renderState);
		}
#endif
	}
}
