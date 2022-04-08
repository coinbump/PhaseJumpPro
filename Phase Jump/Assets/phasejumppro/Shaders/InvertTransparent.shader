/*
    Inverts the texture color
 */
Shader "PhaseJump/InvertTransparent"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
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
             
            struct vertIn {
                float4 vertex : POSITION;
                float2 texcoord : TEXCOORD0;
            };

            struct vertOut {
                float4 vertex : POSITION;
                float2 texcoord : TEXCOORD0;
            };

            vertOut vert(vertIn vi) {
                vertOut vo;
                vo.vertex = UnityObjectToClipPos(vi.vertex);
                vo.texcoord = vi.texcoord;
                return vo;
            }

            half4 frag(vertOut vo) : COLOR {
                half4 texColor = tex2D(_MainTex, vo.texcoord);
                return half4(1.0-texColor.r, 1.0-texColor.g, 1.0-texColor.b, texColor.a);
            }
            ENDCG
        }
    }
}
