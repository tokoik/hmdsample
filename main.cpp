// �E�B���h�E�֘A�̏���
#include "Window.h"

// �W�����C�u����
#include <memory>

// ����
const GgSimpleLight lightData =
{
  { 0.2f, 0.2f, 0.2f, 0.4f },                           // ��������
  { 1.0f, 1.0f, 1.0f, 0.0f },                           // �g�U���ˌ�����
  { 1.0f, 1.0f, 1.0f, 0.0f },                           // ���ʌ�����
  { 0.0f, 0.0f, 1.0f, 1.0f }                            // �ʒu
};

// �ގ�
const GgSimpleMaterial materialData =
{
  { 0.8f, 0.8f, 0.8f, 1.0f },                           // �����̔��ˌW��
  { 0.8f, 0.8f, 0.8f, 1.0f },                           // �g�U���ˌW��
  { 0.2f, 0.2f, 0.2f, 1.0f },                           // ���ʔ��ˌW��
  50.0f                                                 // �P���W��
};

// �O���ʁE�����
const GLfloat zNear(0.1f), zFar(5.0f);

// �I�u�W�F�N�g�̈ʒu
const GLfloat origin[] = { 0.0f, 0.0f, -1.0f, 1.0f };

int main()
{
  // �E�B���h�E���J��
  Window window("HMD Sample", 960, 540);

  // �E�B���h�E���J���Ȃ�������I���
  if (!window.get()) return EXIT_FAILURE;

  // �P���ȉA�e�t�����s���V�F�[�_
  const GgSimpleShader shader("simple.vert", "simple.frag");

  // �\������}�`
  //const std::unique_ptr<const GgElements> obj(ggElementsSphere(0.2f));
  //const std::unique_ptr<const GgTriangles> obj(ggArraysObj("untitled.obj"));
  //const std::unique_ptr<const GgTriangles> obj(ggArraysObj("bunny.obj"));
  //const std::unique_ptr<const GgTriangles> obj(ggArraysObj("HondaS2000.obj"));
  //const std::unique_ptr<const GgElements> obj(ggElementsObj("untitled.obj"));
  //const std::unique_ptr<const GgElements> obj(ggElementsObj("bunny.obj"));
  //const std::unique_ptr<const GgElements> obj(ggElementsObj("HondaS2000.obj"));
  const std::unique_ptr<const GgObj> obj(new GgObj("untitled.obj", shader));
  //const std::unique_ptr<const GgObj> obj(new GgObj("bunny.obj", shader));
  //const std::unique_ptr<const GgObj> obj(new GgObj("HondaS2000.obj", shader));

  // �A�ʏ����������s��
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);

  // �w�i�F
  glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

  // ����
  GgSimpleLightBuffer light(lightData);

  // �ގ�
  GgSimpleMaterialBuffer material(materialData);

  // �E�B���h�E���J���Ă���ԌJ��Ԃ�
  while (!window.shouldClose())
  {
    // ��ʂ���������
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // ���݂̃E�B���h�E�̏c����𒲂ׂ�
    const GLfloat aspect(window.getAspect());

    // ���e�ϊ��s������߂�
    const GgMatrix mp(ggPerspective(1.0f, aspect, zNear, zFar));

    // ����ϊ��s������߂�
    const GgMatrix mv(ggTranslate(origin));

    // ���f���ϊ��s��̓g���b�N�{�[�������Ƃ���
    const GgMatrix mm(window.getLeftTrackball());

    // �`��p�̃V�F�[�_�v���O�����̎g�p�J�n
    shader.use();
    shader.selectLight(light);
    shader.selectMaterial(material);
    shader.loadMatrix(mp, mv * mm);

    // �}�`��`�悷��
    obj->draw();

    // �_�u���o�b�t�@�����O
    window.swapBuffers();
  }
}
