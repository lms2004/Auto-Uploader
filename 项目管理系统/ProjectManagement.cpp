#include"ProjectManament.h"
bool ProjectManagement::shallowCopyProjectFolder(const std::filesystem::path& sourcePath, const std::filesystem::path& destinationPath)
{
	if (!std::filesystem::exists(sourcePath))//���Դ�ļ���ַ·���治����
	{
		std::cout << "����Դ�ļ�·�������ڲ�����" << std::endl;
		return false;
	}

	if (!std::filesystem::exists(destinationPath))//���Ŀ��·����ַ�治����
	{
		std::cout << "����Ŀ���ļ���·��������" << std::endl;
		return false;
	}

	for (const auto& entry : std::filesystem::directory_iterator(destinationPath))//����Ƿ�����ͬ���ļ���
	{
		if (entry.is_regular_file() && entry.path().filename() == sourcePath.filename())
		{
			std::cout << "����Ŀ���ļ��д�����Դ�ļ�����ͬ���ļ���" << std::endl;
			return false;
		}
	}

	try//����sourcePath�����ã����Ƶ�destinationPath��
	{
		std::filesystem::create_directory_symlink(sourcePath, destinationPath);
	}
	catch (const std::filesystem::filesystem_error& ex)
	{
		std::cout << "�����ļ�������ʧ�ܣ�" << ex.what() << std::endl;
		return false;
	}

	std::cout << "ǳ�����ļ��ɹ���" << sourcePath << "->" << destinationPath << std::endl;

	return true;
}

bool ProjectManagement::deleteProjectFolder(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		std::cout << "������Ŀ·��������" << std::endl;
		return false;
	}

	try
	{
		std::filesystem::remove(path);
	}
	catch (const std::filesystem::filesystem_error& ex)
	{
		std::cout << "ɾ����Ŀ�ļ���ʧ�ܣ�" << ex.what() << std::endl;
		return false;
	}

	std::cout << "�ɹ�ɾ����Ŀ�ļ��У�" << path << std::endl;
	return true;
}