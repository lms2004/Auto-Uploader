#include"ProjectManament.h"
bool ProjectManagement::shallowCopyProjectFolder(const std::filesystem::path& sourcePath, const std::filesystem::path& destinationPath)
{
	if (!std::filesystem::exists(sourcePath))//检查源文件地址路径存不存在
	{
		std::cout << "报错：源文件路径不存在不存在" << std::endl;
		return false;
	}

	if (!std::filesystem::exists(destinationPath))//检查目标路径地址存不存在
	{
		std::cout << "报错：目标文件夹路径不存在" << std::endl;
		return false;
	}

	for (const auto& entry : std::filesystem::directory_iterator(destinationPath))//检查是否有相同的文件名
	{
		if (entry.is_regular_file() && entry.path().filename() == sourcePath.filename())
		{
			std::cout << "报错：目标文件夹存在与源文件夹相同的文件名" << std::endl;
			return false;
		}
	}

	try//创建sourcePath的引用，复制到destinationPath上
	{
		std::filesystem::create_directory_symlink(sourcePath, destinationPath);
	}
	catch (const std::filesystem::filesystem_error& ex)
	{
		std::cout << "创建文件夹引用失败：" << ex.what() << std::endl;
		return false;
	}

	std::cout << "浅拷贝文件成功：" << sourcePath << "->" << destinationPath << std::endl;

	return true;
}

bool ProjectManagement::deleteProjectFolder(const std::filesystem::path& path)
{
	if (!std::filesystem::exists(path))
	{
		std::cout << "报错：项目路径不存在" << std::endl;
		return false;
	}

	try
	{
		std::filesystem::remove(path);
	}
	catch (const std::filesystem::filesystem_error& ex)
	{
		std::cout << "删除项目文件夹失败：" << ex.what() << std::endl;
		return false;
	}

	std::cout << "成功删除项目文件夹：" << path << std::endl;
	return true;
}