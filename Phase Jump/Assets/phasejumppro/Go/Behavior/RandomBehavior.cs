using System.Collections;

namespace PJ
{
	/// <summary>
	/// Before evaluating any of its children, it shuffles their order
	/// </summary>
	public class RandomSelectorBehavior : SelectorBehavior
	{
		protected override void EvtStateChanged(SomeStateMachine state)
		{
			if (state != this.state) { return; }

			switch (this.state.State) {
				case State.Evalute:
					children.Shuffle();
					break;
			}

			base.EvtStateChanged(state);
		}
	}

	/// <summary>
	/// Before evaluating any of its children, it shuffles their order
	/// </summary>
	public class RandomSequenceBehavior : SequenceBehavior
	{
		protected override void EvtStateChanged(SomeStateMachine state)
		{
			if (state != this.state) { return; }

			switch (this.state.State)
			{
				case State.Evalute:
					children.Shuffle();
					break;
			}

			base.EvtStateChanged(state);
		}
	}
}
