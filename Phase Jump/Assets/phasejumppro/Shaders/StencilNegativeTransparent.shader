/*
    Uses the red channel of texture as a stencil
 */
Shader "Phase Jump/StencilNegativeTransparent"
{
	Properties
	{
		_MainTex ("Texture", 2D) = "white" {}
        _StencilTex ("Stencil Texture", 2D) = "red" {}
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
            sampler2D _StencilTex;
             
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
                half4 texColor = tex2D(_MainTex, vi.texcoord);
                half4 stencilColor = tex2D(_StencilTex, vi.texcoord);
                
                return half4(texColor.rgb, (1.0-(stencilColor.a*stencilColor.r))*texColor.a);
            }
            ENDCG
        }
    }
}
