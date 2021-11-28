using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using NUnit.Framework;

namespace PJ
{
	public class UnitTests_Command
	{
		private class TestCommand : SomeCommand
		{
			public int value;
			public bool didRedo;

			protected override void ExecuteCommand(bool redo)
			{
				value++;
				didRedo = redo;
			}

			protected override void UndoCommand()
			{
				value--;
			}
		}

		[Test]
		public void UnitTests() {
			var command = new TestCommand();
			command.Execute();
			Assert.AreEqual(1, command.value);
			Assert.IsFalse(command.didRedo);
			command.Undo();
			Assert.AreEqual(0, command.value);
			command.Execute();
			Assert.AreEqual(1, command.value);
			Assert.IsTrue(command.didRedo);
		}
	}

}
