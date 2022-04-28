using System;
using System.Collections.Generic;
using UnityEngine;

/*
 * RATING: 4 stars
 * Tested and works
 * CODE REVIEW: 4/14/22
 */
// FUTURE: add unit tests
namespace PJ
{
    public class WeaponEmitter2D : SomeEmitter
    {
        public class EventEmit : Event
        {
            public List<GameObject> emitList = new List<GameObject>();

            public EventEmit(List<GameObject> emitList)
            {
                this.emitList = emitList;
            }
        }

        protected class BurstTimer : Timer
        {
            public WeakReference<WeaponEmitter2D> owner;

            public BurstTimer(WeaponEmitter2D owner) : base(1.0f, SomeTimed.RunType.RunOnce)
            {
                identifier = "Weapon Burst Timer";
                this.owner = new WeakReference<WeaponEmitter2D>(owner);
            }

            protected override void OnFinish()
            {
                base.OnFinish();

                if (owner.TryGetTarget(out WeaponEmitter2D target))
                {
                    target.OnBurstFinish();
                }
            }
        }

        public bool isEmitterVelocityAdded = false;

        public Broadcaster broadcaster { get; protected set; } = new Broadcaster();

        protected Weapon2D weapon;
        protected Node2D node;
        protected new Rigidbody2D rigidbody2D;
        protected GameObject weaponEmitParent;
        protected Timer weaponBurstTimer;
        protected int weaponBurstCount;
        protected float weaponBurstAngle;

        protected HashSet<Updatable> updatables = new HashSet<Updatable>();

        public Weapon2D Weapon
        {
            get => weapon;
            set
            {
                if (null != weapon)
                {
                    updatables.Remove(weapon);
                }

                weapon = value;
                updatables.Add(weapon);
            }
        }

        protected override void Awake()
        {
            base.Awake();

            node = GetComponent<Node2D>();
            rigidbody2D = GetComponent<Rigidbody2D>();

            weaponBurstTimer = new BurstTimer(this);
            weaponBurstTimer.IsRunning = false;
            updatables.Add(weaponBurstTimer);
        }

        protected override void Start()
        {
            base.Start();

            // Melee weapons need a parent so that their rotation pivot can be altered
            weaponEmitParent = new GameObject();
            weaponEmitParent.transform.parent = transform;
            weaponEmitParent.transform.localPosition = Vector3.zero;
        }

        protected override void Update()
        {
            base.Update();

            var iterUpdatables = new HashSet<Updatable>(updatables);
            foreach (Updatable updatable in iterUpdatables)
            {
                updatable.OnUpdate(new TimeSlice(Time.deltaTime));
            }
        }

        /// <summary>
        /// Call this to fire the weapon, emitting either a bullet, or pattern of bullets, or a melee "bullet"
        /// </summary>
        public List<GameObject> Fire(float directionAngle)
        {
            // Weapon object handles rate limiting timer
            if (!weapon.Fire()) { return new List<GameObject>(); }

            return OnFire(directionAngle, true);
        }

        protected List<GameObject> OnFire(float directionAngle, bool isBurstEnabled)
        {
            var result = new List<GameObject>();

            for (int i = 0; i < weapon.emitCount; i++)
            {
                var bullet = NewBullet();
                if (null == bullet)
                {
                    break;
                }

                result.Add(bullet);

                float angle = 0;
                if (weapon.emitAngles.Count > 0)
                {
                    var angleIndex = i % weapon.emitAngles.Count;
                    angle = weapon.emitAngles[angleIndex];

                    if (weapon.varyEmitAngles.Count > 0)
                    {
                        var varyAngleIndex = i % weapon.varyEmitAngles.Count;
                        float varyAngle = weapon.varyEmitAngles[varyAngleIndex];
                        angle = RandomUtils.VaryFloat(angle, varyAngle);
                    }
                }

                var bulletNode = bullet.GetComponent<Node2D>();
                if (null == bulletNode)
                {
                    Debug.Log("Error. Node required for bullets");
                    return result;
                }

                var weaponAngle = angle + directionAngle;

                /// If requested, destroy the bullet after N seconds (for short range weapons)
                if (weapon.lifeTime > 0)
                {
                    var lifeTimer = bullet.GetComponent<LifeTimer>();
                    if (null == lifeTimer)
                    {
                        lifeTimer = bullet.AddComponent<LifeTimer>();
                    }

                    lifeTimer.lifeTime = weapon.lifeTime;
                }

                switch (weapon.type)
                {
                    case PJ.Weapon.WeaponType.Bullet:
                        // Bullets share the same parent as the object that emitted them
                        bulletNode.transform.parent = gameObject.transform.parent;

                        var bulletVelocityValue = RandomUtils.VaryFloat(weapon.velocity, weapon.varyVelocity);
                        bulletNode.RotationDegreeAngle = weaponAngle;

                        // In the real world, emitter velocity is taken to account
                        // but this can look strange in some games
                        if (isEmitterVelocityAdded && null != rigidbody2D)
                        {
                            var emitterBaseVelocity = rigidbody2D.velocity;
                            var forwardBulletVelocity = AngleUtils.DegreeAngleToVector2(weaponAngle, bulletVelocityValue);
                            forwardBulletVelocity += emitterBaseVelocity;
                            bulletVelocityValue = forwardBulletVelocity.magnitude;
                        }
                        bulletNode.ForwardVelocity = bulletVelocityValue;

                        switch (weapon.fireType)
                        {
                            case PJ.Weapon.FireType.Burst:
                                // Don't burst again if this is a burst fire (infinite)
                                if (isBurstEnabled)
                                {
                                    //Debug.Log("Burst fire. Interval: " + weapon.burstInterval.ToString());
                                    weaponBurstAngle = directionAngle;
                                    weaponBurstCount = 1;
                                    weaponBurstTimer.duration = weapon.burstInterval;
                                    weaponBurstTimer.Reset();
                                    //Debug.Log("Burst fire. Duration: " + weaponBurstTimer.duration.ToString() + " IsRunning: " + weaponBurstTimer.IsRunning.ToString());
                                }
                                break;
                        }
                        break;
                    // Melee weapons stay with the character and are child objects
                    case PJ.Weapon.WeaponType.Melee:
                        weaponEmitParent.transform.eulerAngles = new Vector3(0, 0, 0);
                        bullet.transform.parent = weaponEmitParent.transform;
                        bullet.transform.localPosition = new Vector3(0, 0.5f, 0);
                        weaponEmitParent.transform.eulerAngles = new Vector3(0, 0, -weaponAngle);
                        break;
                }
            }

            // Let listeners add tags to bullet, play audio, etc.
            if (result.Count > 0)
            {
                broadcaster.Broadcast(new EventEmit(result));
            }

            return result;
        }

        protected GameObject NewBullet()
        {
            return Emit();
        }

        protected void OnBurstFinish()
        {
            //Debug.Log("OnBurstFinish: " + weaponBurstCount.ToString());

            if (weaponBurstCount >= weapon.burstCount)
            {
                //Debug.Log("Done Bursting: " + weaponBurstCount);

                weaponBurstTimer.IsRunning = false;
            }
            else
            {
                weaponBurstCount++;
                weaponBurstTimer.Reset();

                // Don't reburst during a burst (infinite burst)
                OnFire(weaponBurstAngle, false);
            }
        }
    }
}
