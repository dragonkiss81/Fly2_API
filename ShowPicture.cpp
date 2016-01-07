void showPicture(FnSprite &sp, char* imageName, int size_x, int size_y,int pos_x, int pos_y)
{
	sp.SetSize(size_x, size_y);
	sp.SetImage(imageName, 0, NULL, 0, NULL, NULL, MANAGED_MEMORY, FALSE, FALSE);
	sp.SetPosition(pos_x, pos_y, 0);
}