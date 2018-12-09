Shader "Unlit/Phase Jump/ToColorTransparent"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
        _Color ("color", Color) = (1.0, 1.0, 1.0, 1.0)
	}
    SubShader {
        Tags {"Queue"="Transparent" "RenderType"="Transparent"}
        LOD 100
        
        ZWrite Off
        Blend SrcAlpha OneMinusSrcAlpha
        
        Pass {
            CGPROGRAM
            
            #pragma vertex vert
            #pragma fragment frag
            
            sampler2D _MainTex;
            fixed4 _Color;
             
            struct vertIn {
                float4 vertex : POSITION;
                float2 texcoord : TEXCOORD0;
            };

            struct vertOut {
                float4 vertex : POSITION;
                float2 texcoord : TEXCOORD0;
            };

            vertOut vert(vertIn i) {
                vertOut o;
                o.vertex = UnityObjectToClipPos(i.vertex);
                o.texcoord = i.texcoord;
                return o;
            }

            half4 frag(vertOut i) : COLOR {
                half4 texColor = tex2D(_MainTex, i.texcoord);
                return half4(_Color[0]*texColor[3],_Color[1]*texColor[3],_Color[2]*texColor[3],_Color[3]*texColor[3]);
            }
            ENDCG
        }
    }
}
