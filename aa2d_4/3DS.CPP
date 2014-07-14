#include "stdafx.h"
#include "3ds.h"


/************************************************************/
//全局变量引用声明：
extern CLoad3DS g_load3DS;
extern t3DModel g_3DModel;
extern UINT g_Texture[MAX_TEXTURES];
extern GLenum g_ViewMode;	// = GL_TRIANGLES; //GL_POINTS, GL_LINE_STRIP

/************************************************************/

/******************************以下为3D模型格式定义类**********************************/
CVector3f::CVector3f(){
	x = y = z = 0.0;
}
CVector3f::CVector3f(float x, float y, float z){
	this->x = x;
	this->y = y;
	this->z = z;
}
CVector3f::CVector3f(const CVector3f& v){
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
}
CVector3f& CVector3f::operator=(const CVector3f& v){
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	return *this;
}
bool CVector3f::operator==(const CVector3f& v){
	if( abs(this->x-v.x)<1e-7 && abs(this->y-v.y)<1e-7 && abs(this->z-v.z)<1e-7 ){
		return true;
	}
	return false;
}
CVector2::CVector2(){
	x = y = 0.0;
}

tFace::tFace(){
	vertIndex[0] = vertIndex[1] = vertIndex[2] = 0;
	coordIndex[0] = coordIndex[1] = coordIndex[2] = 0;
}
tFace::tFace(int index1, int index2, int index3){
	vertIndex[0] = index1;
	vertIndex[1] = index2;
	vertIndex[2] = index3;
	coordIndex[0] = coordIndex[1] = coordIndex[2] = 0;
}
tFace::tFace(const tFace& face){
	for(int i=0; i<3; i++){
		this->vertIndex[i] = face.coordIndex[i];
		this->coordIndex[i] = face.coordIndex[i];
	}
}
tFace& tFace::operator=(const tFace& face){
	for(int i=0; i<3; i++){
		this->vertIndex[i] = face.coordIndex[i];
		this->coordIndex[i] = face.coordIndex[i];
	}
	return *this;
}

tMaterialInfo::tMaterialInfo(){
	memset(strName, 0, sizeof(char)*255);
	memset(strFile, 0, sizeof(char)*255);
	color[0] = color[1] = color[2] = 0;
	texureId = 0;
	uTile = 0.0;
	vTile = 0.0;
	uOffset = 0.0;
	vOffset = 0.0;
}
//  对象信息类
t3DObject::t3DObject(){
	numOfVerts = 0;
	numOfFaces = 0;
	numTexVertex = 0;
	materialID = 0;
	bHasTexture = false;
	memset(strName, 0, sizeof(char)*255);
	pVerts = NULL;
	pFaces = NULL;

	pNormals = NULL;
	pFaceNormals = NULL;
	pTexVerts = NULL;
}
t3DObject::~t3DObject(){
	//SAFE_DELETE_ARRAY(pVerts);
	//SAFE_DELETE_ARRAY(pFaces);
	//SAFE_DELETE_ARRAY(pNormals);
	//SAFE_DELETE_ARRAY(pFaceNormals);
	//SAFE_DELETE_ARRAY(pTexVerts);	
}
void t3DObject::CleanUp(){
	SAFE_DELETE_ARRAY(pVerts);
	SAFE_DELETE_ARRAY(pFaces);
	SAFE_DELETE_ARRAY(pNormals);
	SAFE_DELETE_ARRAY(pFaceNormals);
	SAFE_DELETE_ARRAY(pTexVerts);	
}

t3DObject::t3DObject(const t3DObject& obj){
	this->numOfVerts = obj.numOfVerts;
	this->numOfFaces = obj.numOfFaces;
	this->numTexVertex = obj.numTexVertex;
	this->materialID = obj.materialID;
	this->bHasTexture =obj.bHasTexture;
	strcpy(this->strName, obj.strName);

	this->pVerts = obj.pVerts;
	this->pNormals = obj.pNormals;
	this->pFaces = obj.pFaces;
	this->pFaceNormals = obj.pFaceNormals;
	this->pTexVerts = obj.pTexVerts;		//可能需要改
}
t3DObject& t3DObject::operator=(const t3DObject& obj){
	this->numOfVerts = obj.numOfVerts;
	this->numOfFaces = obj.numOfFaces;
	this->numTexVertex = obj.numTexVertex;
	this->materialID = obj.materialID;
	this->bHasTexture =obj.bHasTexture;
	strcpy(this->strName, obj.strName);

	this->pVerts = obj.pVerts;
	this->pNormals = obj.pNormals;
	this->pFaces = obj.pFaces;
	this->pFaceNormals = obj.pFaceNormals;
	this->pTexVerts = obj.pTexVerts;		//可能需要改

	return *this;
}
//  模型信息类
t3DModel::t3DModel(){
	numOfObjects = 0;
	numOfMaterials = 0;
}
t3DModel::~t3DModel(){
	/*
	for(int i=0; i<pMaterials.size(); i++){
		SAFE_DELETE(&pMaterials[i]);
	}
	for(int i=0; i<pObject.size(); i++){
		SAFE_DELETE(&pObject[i]);
	}
	*/
	pMaterials.clear();
	pObject.clear();
}
void t3DModel::CleanUp(){
	if (numOfObjects > 0)
	{
		for(int i = 0; i <numOfObjects; i++)
		{
			// 删除所有的变量
			SAFE_DELETE_ARRAY(pObject[i].pFaces);
			SAFE_DELETE_ARRAY(pObject[i].pNormals);
			SAFE_DELETE_ARRAY(pObject[i].pVerts);
			SAFE_DELETE_ARRAY(pObject[i].pTexVerts);
		}
		pObject.clear();
		pMaterials.clear();
		numOfObjects = 0;
		numOfMaterials = 0;
	}
}
void t3DModel::Show3DModel(GLenum mode){
	// 遍历模型中所有的对象
	for(int i = 0; i <numOfObjects; i++)
	{
		if(pObject.size()<= 0){		// 如果对象的大小小于0，则退出
			break;
		}

		t3DObject *curObject = &pObject[i];	// 获得当前显示的对象

		// 判断该对象是否有纹理映射
		
		if(curObject->bHasTexture) {

			// 打开纹理映射
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			glBindTexture(GL_TEXTURE_2D, g_Texture[curObject->materialID]);
		} else {

			// 关闭纹理映射
			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}
		
		// 开始以g_ViewMode模式绘制
		glBegin(mode);					
		// 遍历所有的面
		for(int j = 0; j < curObject->numOfFaces; j++)
		{
			// 遍历三角形的所有点
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// 获得面对每个点的索引
				int index = curObject->pFaces[j].vertIndex[whichVertex];

				// 给出法向量
				glNormal3f(curObject->pNormals[ index ].x, curObject->pNormals[ index ].y, curObject->pNormals[ index ].z);
				
				// 如果对象具有纹理
				if(curObject->bHasTexture) {

					// 确定是否有UVW纹理坐标
					if(curObject->pTexVerts) {
						glTexCoord2f(curObject->pTexVerts[ index ].x, curObject->pTexVerts[ index ].y);
					}
				} 
				else {

					if(g_3DModel.pMaterials.size() && curObject->materialID >= 0) 
					{
						BYTE *pColor = g_3DModel.pMaterials[curObject->materialID].color;
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
				}
				glVertex3f(curObject->pVerts[ index ].x, curObject->pVerts[ index ].y, curObject->pVerts[ index ].z);
			}
		}

		glEnd();								// 绘制结束
	}
}

/***************************以下为3DS文件读取相关类********************************/
// 保存块信息的结构
tChunk::tChunk(){
	ID = 0;
	length = 0;
	bytesRead = 0;
}

CLoad3DS::CLoad3DS(){
	m_vertNum=0;
	m_faceNum=0;
	
	m_CurrentChunk = new tChunk();
	m_TempChunk = new tChunk();
}
CLoad3DS::~CLoad3DS(){
	SAFE_DELETE(m_CurrentChunk);
	SAFE_DELETE(m_TempChunk);
}
void CLoad3DS::CleanUp(){
	m_vertNum=0;
	m_faceNum=0;
	if(m_FilePointer!=NULL){
		fclose(m_FilePointer);						// 关闭当前的文件指针
	}
}
//  打开一个3ds文件，读出其中的内容，并释放内存
bool CLoad3DS::Import3DS(t3DModel *pModel, CString strFileName){
	
	char strMessage[255] = {0};

	// 打开一个3ds文件
	m_FilePointer = fopen((LPCTSTR)strFileName, "rb");

	// 确保所获得的文件指针合法
	if(m_FilePointer==NULL) 
	{
		//sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, _T("打开文件错误!"), _T("Error"), MB_OK);
		return false;
	}

	// 当文件打开之后，首先应该将文件最开始的数据块读出以判断是否是一个3ds文件
	// 如果是3ds文件的话，第一个块ID应该是PRIMARY

	// 将文件的第一块读出并判断是否是3ds文件
	ReadChunk(m_CurrentChunk);

	// 确保是3ds文件
	if (m_CurrentChunk->ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, _T("文件格式错误"), _T("Error"), MB_OK);
		return false;
	}

	// 现在开始读入数据,通过调用下面的递归函数，将对象读出
	ProcessNextChunk(pModel, m_CurrentChunk);

	// 在读完整个3ds文件之后，计算顶点的法线
	ComputeNormals(pModel);

	return true;
}

//  下面的函数读出3ds文件的主要部分
void CLoad3DS::ProcessNextChunk(t3DModel *pModel, tChunk *pPreviousChunk){

	t3DObject newObject;				// 用来添加到对象链表
	tMaterialInfo newTexture;				// 用来添加到材质链表
	unsigned int version = 0;					// 保存文件版本
	int buffer[50000] = {0};					// 用来跳过不需要的数据
	memset(buffer, 0, sizeof(int)*50000);

	//  下面每读一个新块，都要判断一下块的ID，如果该块是需要的读入的，则继续进行,如果是不需要读入的块，则略过
	// 继续读入子块，直到达到预定的长度
	m_CurrentChunk = new tChunk();
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// 读入下一个块
		ReadChunk(m_CurrentChunk);

		// 判断块的ID号
		switch (m_CurrentChunk->ID)
		{
		case VERSION:							// 文件版本号
			
			// 在该块中有一个无符号短整型数保存了文件的版本
			
			// 读入文件的版本号，并将字节数添加到bytesRead变量中
			m_CurrentChunk->bytesRead += fread(&version, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);

			// 如果文件版本号大于3，给出一个警告信息
			if (version > 0x03)
				MessageBox(NULL, _T("This 3DS file is over version 3 so it may load incorrectly"), _T("Warning"), MB_OK);
			break;

		case OBJECTINFO:						// 网格版本信息
			
			// 读入下一个块
			ReadChunk(m_TempChunk);

			// 获得网格的版本号
			m_TempChunk->bytesRead += fread(&version, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);

			// 增加读入的字节数
			m_CurrentChunk->bytesRead += m_TempChunk->bytesRead;

			// 进入下一个块
			ProcessNextChunk(pModel, m_CurrentChunk);
			break;

		case MATERIAL:							// 材质信息
			// 材质的数目递增
			pModel->numOfMaterials++;

			// 在纹理链表中添加一个空白纹理结构
			pModel->pMaterials.push_back(newTexture);

			// 进入材质装入函数
			ProcessNextMaterialChunk(pModel, m_CurrentChunk);
			break;

		case OBJECT:							// 对象的名称
				
			// 该块是对象信息块的头部，保存了对象了名称

			// 对象数递增
			pModel->numOfObjects++;
		
			// 添加一个新的tObject节点到对象链表中
			pModel->pObject.push_back(newObject);
			
			// 初始化对象和它的所有数据成员
			memset(&(pModel->pObject[pModel->numOfObjects - 1]), 0, sizeof(t3DObject));

			// 获得并保存对象的名称，然后增加读入的字节数
	
			m_CurrentChunk->bytesRead += GetString(pModel->pObject[pModel->numOfObjects - 1].strName);
			
			// 进入其余的对象信息的读入
			ProcessNextObjectChunk(pModel, &(pModel->pObject[pModel->numOfObjects - 1]), m_CurrentChunk);
			break;

		case EDITKEYFRAME:

			// 跳过关键帧块的读入，增加需要读入的字节数
			m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;

		default: 
			
			//  跳过所有忽略的块的内容的读入，增加需要读入的字节数
			m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}

		// 增加从最后块读入的字节数
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
	}

	// 释放当前块的内存空间
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}


//  下面的函数处理所有的文件中对象的信息
void CLoad3DS::ProcessNextObjectChunk(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	int buffer[50000] = {0};					// 用于读入不需要的数据

	m_CurrentChunk = new tChunk();
	// 继续读入块的内容直至本子块结束
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// 读入下一个块
		ReadChunk(m_CurrentChunk);

		// 区别读入是哪种块
		switch (m_CurrentChunk->ID)
		{
		case OBJECT_MESH:					// 正读入的是一个新块
		
			// 使用递归函数调用，处理该新块
			ProcessNextObjectChunk(pModel, pObject, m_CurrentChunk);
			break;

		case OBJECT_VERTICES:				// 读入是对象顶点
			ReadVertices(pObject, m_CurrentChunk);

			//ColModelNum(pObject, m_CurrentChunk);
			break;

		case OBJECT_FACES:					// 读入的是对象的面
			ReadVertexIndices(pObject, m_CurrentChunk);

			//ColModelNum(pObject, m_CurrentChunk);
			break;

		case OBJECT_MATERIAL:				// 读入的是对象的材质名称
			
			// 该块保存了对象材质的名称，可能是一个颜色，也可能是一个纹理映射。同时在该块中也保存了
			// 纹理对象所赋予的面

			// 下面读入对象的材质名称
			ReadObjectMaterial(pModel, pObject, m_CurrentChunk);			
			break;

		case OBJECT_UV:						// 读入对象的UV纹理坐标

			// 读入对象的UV纹理坐标
			ReadUVCoordinates(pObject, m_CurrentChunk);
			break;

		default:  

			// 略过不需要读入的块
			m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}

		// 添加从最后块中读入的字节数到前面的读入的字节中
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;
		
	}

	// 释放当前块的内存空间，并把当前块设置为前面块
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}

//  下面的函数处理所有的材质信息
void CLoad3DS::ProcessNextMaterialChunk(t3DModel *pModel, tChunk *pPreviousChunk)
{
	int buffer[50000] = {0};					// 用于读入不需要的数据

	// 给当前块分配存储空间
	m_CurrentChunk = new tChunk();

	// 继续读入这些块，知道该子块结束
	while (pPreviousChunk->bytesRead < pPreviousChunk->length)
	{
		// 读入下一块
		ReadChunk(m_CurrentChunk);

		// 判断读入的是什么块
		switch (m_CurrentChunk->ID)
		{
		case MATNAME:							// 材质的名称
			
			// 读入材质的名称
			m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strName, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;

		case MATDIFFUSE:						// 对象的R G B颜色
			ReadColorChunk(&(pModel->pMaterials[pModel->numOfMaterials - 1]), m_CurrentChunk);
			break;
		
		case MATMAP:							// 纹理信息的头部
			
			// 进入下一个材质块信息
			ProcessNextMaterialChunk(pModel, m_CurrentChunk);
			break;

		case MATMAPFILE:						// 材质文件的名称

			// 读入材质的文件名称
			m_CurrentChunk->bytesRead += fread(pModel->pMaterials[pModel->numOfMaterials - 1].strFile, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		
		default:  

			// 掠过不需要读入的块
			m_CurrentChunk->bytesRead += fread(buffer, 1, m_CurrentChunk->length - m_CurrentChunk->bytesRead, m_FilePointer);
			break;
		}

		// 添加从最后块中读入的字节数
		pPreviousChunk->bytesRead += m_CurrentChunk->bytesRead;

	}

	// 删除当前块，并将当前块设置为前面的块
	delete m_CurrentChunk;
	m_CurrentChunk = pPreviousChunk;
}

//  下面函数读入块的ID号和它的字节长度
void CLoad3DS::ReadChunk(tChunk *pChunk)
{
	// 读入块的ID号，占用了2个字节。块的ID号象OBJECT或MATERIAL一样，说明了在块中所包含的内容
	pChunk->bytesRead = fread(&pChunk->ID, 1, 2, m_FilePointer);

	// 然后读入块占用的长度，包含了四个字节
	pChunk->bytesRead += fread(&pChunk->length, 1, 4, m_FilePointer);
}

//  下面的函数读入一个字符串
int CLoad3DS::GetString(char *pBuffer)
{
	int index = 0;

	// 读入一个字节的数据
	fread(pBuffer, 1, 1, m_FilePointer);
	//m_FilePointer->_ptr = "resource/untit000.bmp";

	// 直到结束
	while (*(pBuffer + index++) != 0) {

		// 读入一个字符直到NULL
		fread(pBuffer + index, 1, 1, m_FilePointer);
	}

	// 返回字符串的长度
	return strlen(pBuffer) + 1;
}


//  下面的函数读入RGB颜色
void CLoad3DS::ReadColorChunk(tMaterialInfo *pMaterial, tChunk *pChunk)
{
	// 读入颜色块信息
	ReadChunk(m_TempChunk);

	// 读入RGB颜色
	m_TempChunk->bytesRead += fread(pMaterial->color, 1, m_TempChunk->length - m_TempChunk->bytesRead, m_FilePointer);

	// 增加读入的字节数
	pChunk->bytesRead += m_TempChunk->bytesRead;
}

//  下面的函数读入顶点索引
void CLoad3DS::ReadVertexIndices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	unsigned short index = 0;					// 用于读入当前面的索引

	// 读入该对象中面的数目
	pPreviousChunk->bytesRead += fread(&pObject->numOfFaces, 1, 2, m_FilePointer);

	// 分配所有面的存储空间，并初始化结构
	pObject->pFaces = new tFace [pObject->numOfFaces];
	memset(pObject->pFaces, 0, sizeof(tFace) * pObject->numOfFaces);

	// 遍历对象中所有的面
	for(int i = 0; i < pObject->numOfFaces; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			// 读入当前面的第一个点 
			pPreviousChunk->bytesRead += fread(&index, 1, sizeof(index), m_FilePointer);

			if(j < 3)
			{
				// 将索引保存在面的结构中
				pObject->pFaces[i].vertIndex[j] = index;
			}
		}
	}
}


//  下面的函数读入对象的UV坐标
void CLoad3DS::ReadUVCoordinates(t3DObject *pObject, tChunk *pPreviousChunk)
{
	// 为了读入对象的UV坐标，首先需要读入UV坐标的数量，然后才读入具体的数据

	// 读入UV坐标的数量
	pPreviousChunk->bytesRead += fread(&pObject->numTexVertex, 1, 2, m_FilePointer);

	// 分配保存UV坐标的内存空间
	pObject->pTexVerts = new CVector2 [pObject->numTexVertex];

	// 读入纹理坐标
	pPreviousChunk->bytesRead += fread(pObject->pTexVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}

//  读入对象的顶点
void CLoad3DS::ReadVertices(t3DObject *pObject, tChunk *pPreviousChunk)
{
	// 在读入实际的顶点之前，首先必须确定需要读入多少个顶点。
	
	// 读入顶点的数目
	pPreviousChunk->bytesRead += fread(&(pObject->numOfVerts), 1, 2, m_FilePointer);

	// 分配顶点的存储空间，然后初始化结构体
	pObject->pVerts = new CVector3f [pObject->numOfVerts];
	memset(pObject->pVerts, 0, sizeof(CVector3f) * pObject->numOfVerts);

	// 读入顶点序列
	pPreviousChunk->bytesRead += fread(pObject->pVerts, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);

	// 现在已经读入了所有的顶点。
	// 因为3D Studio Max的模型的Z轴是指向上的，因此需要将y轴和z轴翻转过来。
	// 具体的做法是将Y轴和Z轴交换，然后将Z轴反向。

	// 遍历所有的顶点
	for(int i = 0; i < pObject->numOfVerts; i++)
	{
		// 保存Y轴的值
		float fTempY = pObject->pVerts[i].y;

		// 设置Y轴的值等于Z轴的值
		pObject->pVerts[i].y = pObject->pVerts[i].z;

		// 设置Z轴的值等于-Y轴的值 
		pObject->pVerts[i].z =fTempY;
	}
}


//  下面的函数读入对象的材质名称
void CLoad3DS::ReadObjectMaterial(t3DModel *pModel, t3DObject *pObject, tChunk *pPreviousChunk)
{
	char strMaterial[255] = {0};			// 用来保存对象的材质名称
	int buffer[50000] = {0};				// 用来读入不需要的数据
	

	// 材质或者是颜色，或者是对象的纹理，也可能保存了象明亮度、发光度等信息。

	// 下面读入赋予当前对象的材质名称
	pPreviousChunk->bytesRead += GetString(strMaterial);

	// 遍历所有的纹理
	for(int i = 0; i < pModel->numOfMaterials; i++)
	{
		//如果读入的纹理与当前的纹理名称匹配
		if(strcmp(strMaterial, pModel->pMaterials[i].strName) == 0)
		{
			// 设置材质ID
			pObject->materialID = i;
			

			// 判断是否是纹理映射，如果strFile是一个长度大于1的字符串，则是纹理
			if(strlen(pModel->pMaterials[i].strFile) > 0) {
				

				// 设置对象的纹理映射标志
				pObject->bHasTexture = true;
			
				//pModel->pMaterials[i].strName = "resource/untit000.bmp";
				//pObject->strName+=
			}	
			break;
		}
		else
		{
			// 如果该对象没有材质，则设置ID为-1
			pObject->materialID = -1;
		}
	}

	pPreviousChunk->bytesRead += fread(buffer, 1, pPreviousChunk->length - pPreviousChunk->bytesRead, m_FilePointer);
}			


/************************************************************************/
/* 
	下面的这些函数主要用来计算顶点的法向量，顶点的法向量主要用来计算光照
*/
/************************************************************************/
// 下面计算一个矢量的长度
float Mag(CVector3f v){
	return sqrt(v.x*v.x + v.y*v.y + v.z*v.z);
}

//计算三角形面积，参数为三条边矢量
float TrangleArea(const CVector3f& v1,  const CVector3f& v2, const CVector3f& v3){
	float len1 = Mag(v1);
	float len2 = Mag(v2);
	float len3 = Mag(v3);
	float s = (len1+len2+len3) / 2;
	if(s<eps){
		return 0.0;
	}
	else{
		return sqrt( s*(s-len1)*(s-len2)*(s-len3) );
	}
}

// 下面的函数求两点决定的矢量
CVector3f Vector2(const CVector3f& vPoint1, const CVector3f& vPoint2)
{		
	return CVector3f(vPoint1.x-vPoint2.x, vPoint1.y-vPoint2.y, vPoint1.z-vPoint2.z);								
}

// 下面的函数两个矢量相加
CVector3f AddVector(CVector3f& vVector1, CVector3f& vVector2)
{
	return CVector3f(vVector2.x+vVector1.x, vVector2.y+vVector1.y, vVector2.z+vVector1.z);					
}

// // 下面的函数两个矢量相减
CVector3f SubVector(CVector3f& vVector1, CVector3f& vVector2){
	return CVector3f(vVector2.x-vVector1.x, vVector2.y-vVector1.y, vVector2.z-vVector1.z);
}
// 下面的函数得到两个矢量的点积
float Dot(CVector3f& v1, CVector3f& v2){
	return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}
//下面的函数将一个矢量除以一个常数
CVector3f DivideByNum(CVector3f& v, float num){
	return CVector3f(v.x/num, v.y/num, v.z/num);
}
//下面的函数将一个矢量除以一个常数
CVector3f MultiplyByNum(CVector3f& v, float num){
	return CVector3f(v.x * num, v.y * num, v.z * num);
}
// 下面的函数处理矢量的缩放
CVector3f DivideVectorByScaler(CVector3f& vVector1, float Scaler)
{
	CVector3f vResult;							
	vResult.x = vVector1.x / Scaler;			
	vResult.y = vVector1.y / Scaler;			
	vResult.z = vVector1.z / Scaler;			
	return vResult;								
}



// 下面的函数返回两个矢量的叉积
CVector3f Cross(CVector3f& vVector1, CVector3f& vVector2)
{
	CVector3f vCross;								

	vCross.x = ((vVector1.y * vVector2.z) - (vVector1.z * vVector2.y));
	vCross.y = ((vVector1.z * vVector2.x) - (vVector1.x * vVector2.z));
	vCross.z = ((vVector1.x * vVector2.y) - (vVector1.y * vVector2.x));

	return vCross;								
}

// 下面的函数规范化矢量
CVector3f Normalize(CVector3f& vNormal)
{
	double Magnitude = Mag(vNormal);// 获得矢量的长度
	if((Magnitude-0.0)<eps){
		vNormal.x = vNormal.y = vNormal.z = 0.0;
	}
	else{
		vNormal.x /= (float)Magnitude;				
		vNormal.y /= (float)Magnitude;				
		vNormal.z /= (float)Magnitude;	
	}		
	return vNormal;								
}

//计算两个矢量之间的夹角[0-180]
float AngleBetweenVector(CVector3f& v1, CVector3f& v2){
	float denominator = Mag(v1)*Mag(v2);
	denominator = denominator>eps ? denominator : eps;
	float angle = Dot(v1, v2);
	angle = abs(angle)>eps ? angle : eps;
	angle = Dot(v1, v2) / denominator;
	angle = angle<-1.0 ? -1.0 : angle;
	angle = angle >1.0 ? 1.0 : angle;
	angle = acos( angle );
	return angle;
}
//向量取反
CVector3f MinusVector(CVector3f& v){
	CVector3f vVector;
	vVector.x = -v.x;
	vVector.y = -v.y;
	vVector.z = -v.z;
	return vVector;
}


//  下面的函数用于计算对象的法向量
void CLoad3DS::ComputeNormals(t3DModel *pModel)
{
	CVector3f vVector1, vVector2, vNormal, vPoly[3];

	// 如果模型中没有对象，则返回
	if(pModel->numOfObjects <= 0)
		return;

	// 遍历模型中所有的对象
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		// 获得当前的对象
		t3DObject *pObject = &(pModel->pObject[index]);
		Objects[index]  = &(pModel->pObject[index]);

		// 分配需要的存储空间
		pObject->pFaceNormals = new CVector3f [pObject->numOfFaces];
		
		CVector3f *pTempNormals	= new CVector3f [pObject->numOfFaces];
		pObject->pNormals		= new CVector3f [pObject->numOfVerts];

		// 遍历对象的所有面
		for(int i=0; i < pObject->numOfFaces; i++)
		{												
			vPoly[0] = pObject->pVerts[pObject->pFaces[i].vertIndex[0]];
			vPoly[1] = pObject->pVerts[pObject->pFaces[i].vertIndex[1]];
			vPoly[2] = pObject->pVerts[pObject->pFaces[i].vertIndex[2]];

			// 计算面的法向量

			vVector1 = Vector2(vPoly[0], vPoly[2]);		// 获得多边形的矢量
			vVector2 = Vector2(vPoly[2], vPoly[1]);		// 获得多边形的第二个矢量

			vNormal  = Cross(vVector1, vVector2);		// 获得两个矢量的叉积
			pTempNormals[i] = vNormal;					// 保存非规范化法向量
			vNormal  = Normalize(vNormal);				// 规范化获得的叉积

			pObject->pFaceNormals[i] = vNormal;						// 将法向量添加到法向量列表中

		}

		//  下面求顶点法向量
		CVector3f vSum;
		CVector3f vZero = vSum;
		int shared=0;
		// 遍历所有的顶点
		for (int i = 0; i < pObject->numOfVerts; i++)			
		{
			for (int j = 0; j < pObject->numOfFaces; j++)	// 遍历所有的三角形面
			{												// 判断该点是否与其它的面共享
				if (pObject->pFaces[j].vertIndex[0] == i || 
					pObject->pFaces[j].vertIndex[1] == i || 
					pObject->pFaces[j].vertIndex[2] == i)
				{
					vSum = AddVector(vSum, pTempNormals[j]);
					shared++;								
				}
			}      
			
			pObject->pNormals[i] = DivideVectorByScaler(vSum, float(-shared));

			// 规范化最后的顶点法向
			pObject->pNormals[i] = Normalize(pObject->pNormals[i]);	

			vSum = vZero;								
			shared = 0;										
		}
	
		// 释放存储空间，开始下一个对象
		delete [] pTempNormals;
	}
}






/************************************************************/
//计算材质相关的函数

void CLoad3DS::CreateTexture(UINT textureArray[], LPSTR strFileName, int textureID)
{
	
	AUX_RGBImageRec *pBitmap = NULL;
	
	if(!strFileName)									// 如果无此文件，则直接返回
		return;

	CString strFileName0 = strFileName;
	CString strFileName1= pathName+driveName+strFileName0;
	
	pBitmap = auxDIBImageLoad(strFileName);	
	
	if(pBitmap == NULL)									// 如果装入位图失败，则退出
		exit(0);

	// 生成纹理
	glGenTextures(1, &textureArray[textureID]);

	// 设置像素对齐格式
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

	glBindTexture(GL_TEXTURE_2D, textureArray[textureID]);

	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, pBitmap->sizeX, pBitmap->sizeY, GL_RGB, GL_UNSIGNED_BYTE, pBitmap->data);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR_MIPMAP_LINEAR);

	if (pBitmap)										// 释放位图占用的资源
	{
		if (pBitmap->data)						
		{
			free(pBitmap->data);				
		}

		free(pBitmap);					
	}
	
	
}



// 绘制合并后的3d模型
void CLoad3DS::Show3DS(GLenum mode)
{
	for(int i = 0; i < g_3DModel.numOfObjects; i++)
	{
		// 如果对象的大小小于0，则退出
		if(g_3DModel.pObject.size() <= 0){
			break;
		}

		// 获得当前显示的对象
		t3DObject *pObject = &g_3DModel.pObject[i];

		// 判断该对象是否有纹理映射
		if(pObject->bHasTexture) {

			// 打开纹理映射
			glEnable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
			glBindTexture(GL_TEXTURE_2D, g_Texture[pObject->materialID]);
		} 
		else {

			// 关闭纹理映射
			glDisable(GL_TEXTURE_2D);
			glColor3ub(255, 255, 255);
		}
		// 开始以g_ViewMode模式绘制
		glBegin(mode);					
		// 遍历所有的面
		for(int j = 0; j < pObject->numOfFaces; j++)
		{
			// 遍历三角形的所有点
			for(int whichVertex = 0; whichVertex < 3; whichVertex++)
			{
				// 获得面对每个点的索引
				int index = pObject->pFaces[j].vertIndex[whichVertex];

				// 给出法向量
				glNormal3f(pObject->pNormals[ index ].x, pObject->pNormals[ index ].y, pObject->pNormals[ index ].z);

				// 如果对象具有纹理
				/*if(showTextureFlag == 1)
				{*/
				if(pObject->bHasTexture) {

					// 确定是否有UVW纹理坐标
					if(pObject->pTexVerts) {
						glTexCoord2f(pObject->pTexVerts[ index ].x, pObject->pTexVerts[ index ].y);
					}
				} else {

					if(g_3DModel.pMaterials.size()>=0 && pObject->materialID >= 0) 
					{
						BYTE *pColor = g_3DModel.pMaterials[pObject->materialID].color;
						glColor3ub(pColor[0], pColor[1], pColor[2]);
					}
					//}
				}
				glVertex3f(pObject->pVerts[ index ].x, pObject->pVerts[ index ].y, pObject->pVerts[ index ].z);
			}
		}

		glEnd();								// 绘制结束
	}

}

bool  CLoad3DS::ImporToDabase(t3DModel *pModel, CString strFileName)
{
	
	char strMessage[255] = {0};

	// 打开一个3ds文件
	m_FilePointer = fopen((LPSTR)(LPCTSTR)strFileName, "rb");

	// 确保所获得的文件指针合法
	if(!m_FilePointer) 
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		//MessageBox(NULL, _T(strMessage), "Error", MB_OK);
		return false;
	}

	// 当文件打开之后，首先应该将文件最开始的数据块读出以判断是否是一个3ds文件
	// 如果是3ds文件的话，第一个块ID应该是PRIMARY

	// 将文件的第一块读出并判断是否是3ds文件
	ReadChunk(m_CurrentChunk);

	// 确保是3ds文件
	if (m_CurrentChunk->ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, _T("文件格式错误"), _T("Error"), MB_OK);
		return false;
	}

	// 现在开始读入数据，ProcessNextChunk()是一个递归函数

	// 通过调用下面的递归函数，将对象读出
	ProcessNextChunk(pModel, m_CurrentChunk);

	// 在读完整个3ds文件之后，计算顶点的法线
	//ColModelNum(pObject->;
	//pObject->pFaces = new tFace [pObject->];

	ComputeNormals(pModel);

	// 释放内存空间
	//CleanUp();

	return true;
	
}


bool  CLoad3DS::ColModelNum(t3DModel *pModel,CString strFileName,long& faceNum,long&vertNum)
{

    char strMessage[255] = {0};

	// 打开一个3ds文件
	m_FilePointer = fopen((LPSTR)(LPCTSTR)strFileName, "rb");

	// 确保所获得的文件指针合法
	if(!m_FilePointer) 
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, _T("打开文件错误"), _T("Error"), MB_OK);
		return false;
	}

	// 当文件打开之后，首先应该将文件最开始的数据块读出以判断是否是一个3ds文件
	// 如果是3ds文件的话，第一个块ID应该是PRIMARY

	// 将文件的第一块读出并判断是否是3ds文件
	ReadChunk(m_CurrentChunk);

	// 确保是3ds文件
	if (m_CurrentChunk->ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, _T("文件格式错误"), _T("Error"), MB_OK);
		return false;
	}

	// 现在开始读入数据，ProcessNextChunk()是一个递归函数

	// 通过调用下面的递归函数，将对象读出
	ProcessNextChunk(pModel, m_CurrentChunk);

	// 如果模型中没有对象，则返回
	if(pModel->numOfObjects <= 0)
		return false;

	// 遍历模型中所有的对象
	for(int index = 0; index < pModel->numOfObjects; index++)
	{
		// 获得当前的对象
		t3DObject *pObject = &(pModel->pObject[index]);
		//Objects[index]  = &(pModel->pObject[index]);

		// 分配需要的存储空间
		faceNum += pObject->numOfFaces;
		vertNum += pObject ->numOfVerts;
	
		/*sprintf(strMessage, "Unable to load PRIMARY chuck from file: %d!", faceNum);
		MessageBox(NULL, strMessage, "result", MB_OK);*///判断是否读出模型点的个数
		
	}
	return true;
}


bool  CLoad3DS::ColModel(t3DModel *pModel,CString strFileName)
{
	char strMessage[255] = {0};

	// 打开一个3ds文件
	m_FilePointer = fopen((LPSTR)(LPCTSTR)strFileName, "rb");

	// 确保所获得的文件指针合法
	if(!m_FilePointer) 
	{
		sprintf(strMessage, "Unable to find the file: %s!", strFileName);
		MessageBox(NULL, _T("打开文件错误"), _T("Error"), MB_OK);
		return false;
	}

	// 当文件打开之后，首先应该将文件最开始的数据块读出以判断是否是一个3ds文件
	// 如果是3ds文件的话，第一个块ID应该是PRIMARY

	// 将文件的第一块读出并判断是否是3ds文件
	ReadChunk(m_CurrentChunk);

	// 确保是3ds文件
	if (m_CurrentChunk->ID != PRIMARY)
	{
		sprintf(strMessage, "Unable to load PRIMARY chuck from file: %s!", strFileName);
		MessageBox(NULL, _T("文件格式错误"), _T("Error"), MB_OK);
		return false;
	}

	// 现在开始读入数据，ProcessNextChunk()是一个递归函数

	// 通过调用下面的递归函数，将对象读出
	ProcessNextChunk(pModel, m_CurrentChunk);
	return true;
}
bool CLoad3DS::ColVertsCoordinate(t3DModel *pModel,CString strFileName,float** vertsCoordinate)
{
	
	int j = 0;

	ColModel(pModel,strFileName);
	

	for(int i = 0; i < pModel->numOfObjects; i++)
	{

		for(; j < pModel->pObject[i].numOfVerts; j++ )
		{

			
				vertsCoordinate[j][0] = pModel->pObject[i].pVerts[j].x;
				vertsCoordinate[j][1] = pModel->pObject[i].pVerts[j].y;
				vertsCoordinate[j][2] = pModel->pObject[i].pVerts[j].z;

		}
	}
	return true;
}

bool CLoad3DS::ColNormalsCoordinate(t3DModel *pModel,CString strFileName,float** normalCoordinate)
{
	ColModel(pModel,strFileName);
	ComputeNormals(pModel);

	for(int i = 0; i < pModel->numOfObjects; i++)
	{
	
		for(int j = 0; j < pModel->pObject[i].numOfVerts; j++ )
		{
				normalCoordinate[j][0] = pModel->pObject[i].pNormals[j].x;
				normalCoordinate[j][1] = pModel->pObject[i].pNormals[j].y;
				normalCoordinate[j][2] = pModel->pObject[i].pNormals[j].z;

		}
	}
	return true;
}
bool CLoad3DS::ColMidleVert(t3DModel *pModel,CString strFileName,float& midVertz)
{
	ColModel(pModel,strFileName);

	if(pModel->numOfObjects <= 0)
		return false;
	float maxVertx,maxVerty,maxVertz;
	float minVertx,minVerty,minVertz;

	maxVertx = 0.0;
	maxVerty = 0.0;
	maxVertz = 0.0;
	minVertx = 0.0;
	minVerty = 0.0;
	minVertz = 0.0;
	


	// 遍历模型中所有的对象
	for(int i = 0; i < pModel->numOfObjects; i++)
	{
		// 获得当前的对象
		for(int j = 0 ; j < pModel->pObject[i].numOfVerts; j++)
		{
			if(pModel->pObject[i].pVerts[j].z > maxVertz)
			{
				maxVertz = pModel->pObject[i].pVerts[j].z;
			}
			else
			{
				minVertz = pModel->pObject[i].pVerts[j].z;
			}
		}
	}
	midVertz = (maxVertz + minVertz)/2;
}