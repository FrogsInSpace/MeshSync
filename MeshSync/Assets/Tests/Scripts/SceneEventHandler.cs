using System.Collections.Generic;
using System.Linq;
using UnityEngine;
using UTJ.MeshSync;


[ExecuteInEditMode]
[RequireComponent(typeof(MeshSyncServer))]
public class SceneEventHandler : MonoBehaviour
{
    string m_log;

    void OnSceneUpdateBegin()
    {
        m_log = "SceneUpdateBegin\n";
    }
    void OnUpdateTexture(Texture tex, TextureData data)
    {
        m_log += "Texture: " + tex.name + "\n";
    }
    void OnUpdateMaterial(Material mat, MaterialData data)
    {
        m_log += "Material: " + mat.name + "\n";
    }
    void OnUpdateEntity(GameObject obj, TransformData data)
    {
        m_log += "GameObject: " + obj.name + "\n";

    }
    void OnUpdateAnimation(AnimationClip anim, AnimationClipData data)
    {
        m_log += "AnimationClip: " + anim.name + "\n";
    }
    void OnSceneUpdateEnd()
    {
        m_log += "SceneUpdateEnd\n";
        Debug.Log(m_log);
    }


    void OnEnable()
    {
        var mss = GetComponent<MeshSyncServer>();
        if (mss != null)
        {
            mss.onSceneUpdateBegin += OnSceneUpdateBegin;
            mss.onSceneUpdateEnd += OnSceneUpdateEnd;
            mss.onUpdateTexture += OnUpdateTexture;
            mss.onUpdateMaterial += OnUpdateMaterial;
            mss.onUpdateEntity += OnUpdateEntity;
            mss.onUpdateAnimation += OnUpdateAnimation;
        }
    }

    void OnDisable()
    {
        var mss = GetComponent<MeshSyncServer>();
        if (mss != null)
        {
            mss.onSceneUpdateBegin -= OnSceneUpdateBegin;
            mss.onSceneUpdateEnd -= OnSceneUpdateEnd;
            mss.onUpdateTexture -= OnUpdateTexture;
            mss.onUpdateMaterial -= OnUpdateMaterial;
            mss.onUpdateEntity -= OnUpdateEntity;
            mss.onUpdateAnimation -= OnUpdateAnimation;
        }
    }
}

