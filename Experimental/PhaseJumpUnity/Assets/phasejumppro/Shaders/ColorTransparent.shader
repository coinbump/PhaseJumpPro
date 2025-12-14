/*
    Renders a uniform color, with alpha
 */
Shader "PhaseJump/ColorTransparent"
{
	Properties
	{
        _Color ("Color", Color) = (1.0, 1.0, 1.0, 1.0)
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
            
            fixed4 _Color;
             
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

            half4 frag(vertOut vi) : COLOR {
                return half4(_Color[0],_Color[1],_Color[2],_Color[3]);
            }
            ENDCG
        }
    }
}
