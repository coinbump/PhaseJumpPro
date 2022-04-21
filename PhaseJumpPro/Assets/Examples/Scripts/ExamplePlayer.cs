using System.Collections;
using System.Collections.Generic;
using PJ;
using UnityEngine;
using UnityEngine.InputSystem;
using UnityEngine.SceneManagement;
using TMPro;

public class ExamplePlayer : Node2D
{
    public class Model : PJ.Core<Model.StateType>
    {
        public const int MaxHealth = 5;

        public enum StateType
        {
            Default,
            Dead
        }

        public PublishedValue<int> health { get; protected set; } = new PublishedValue<int>(MaxHealth);

        public Model()
        {
        }
    }

    public Model model = new Model();
    protected Weapon2D burstWeapon = new Weapon2D();
    protected WeaponEmitter2D weaponEmitter;

    public bool IsDead
    {
        get => model.StateMachine.State == Model.StateType.Dead;
    }

    protected override void Awake()
    {
        base.Awake();

        model.health.AddListener(this);

        burstWeapon.EmitSpread(3, 2, 2);
        burstWeapon.velocity = 20.0f;
        burstWeapon.limitTime = 0.4f;
        burstWeapon.burstCount = 3;
        burstWeapon.burstInterval = 0.1f;
        burstWeapon.fireType = PJ.Weapon.FireType.Burst;

        weaponEmitter = GetComponent<WeaponEmitter2D>();
        weaponEmitter.Weapon = burstWeapon;
    }

    protected override void Start()
    {
        base.Start();
    }

    protected override void OnUpdate(TimeSlice time)
    {
        base.OnUpdate(time);
    }

    public void OnInputMoveRight(InputAction.CallbackContext context)
    {
        var inputVector = context.ReadValue<Vector2>();

        var distance = AngleUtils.Hypotenuse(inputVector.x, inputVector.y);
        //Debug.Log("MoveRight Distance: " + distance.ToString());

        var minDistance = 0.3f;
        if (distance < minDistance)
        {
            return;
        }

        var angle = AngleUtils.Vector2ToDegreeAngle(inputVector);
        weaponEmitter.Fire(angle);
    }

    protected void OnHealthChange(int health)
    {
        Debug.Log("ExamplePlayer OnHealthChange");

    }

    public override void OnListen(PJ.Event theEvent)
    {
        base.OnListen(theEvent);

        model.health.OnValueChange(theEvent, OnHealthChange);
    }
}
