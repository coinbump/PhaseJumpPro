using System;
using System.Collections;
using System.Diagnostics;
using UnityEngine;

/*
 * RATING: 5 stars
 * Simple utility
 * CODE REVIEW: 12/29/22
 */
namespace PJ
{
    /// <summary>
    /// Profiler. Must use via using syntax in C# so Dispose is called
    /// </summary>
    /// <example>
    /// using (new PJ.Profiler("TestCase"))
    /// </example>
    class Profiler : IDisposable
    {
        protected string name = "";
        protected Stopwatch stopwatch;

        public Profiler(string name)
        {
            this.name = name;
            stopwatch = Stopwatch.StartNew();
        }

        public void Dispose()
        {
            stopwatch.Stop();
            var log = String.Format("PROFILE: {0}\tDURATION: {1}ms", name, stopwatch.ElapsedMilliseconds);
            UnityEngine.Debug.Log(log);
        }
    }
}
