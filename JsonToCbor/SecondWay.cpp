static void toCbor(QVariant& variant, QJsonValue inValue)
{
	if (inValue.isArray())
	{
		QJsonArray arr = inValue.toArray();
		QList<QVariant> list;

		QJsonArray::const_iterator myIterator = arr.constBegin();
		QJsonArray::const_iterator myIteratorEnd = arr.constEnd();
		do
		{
			QJsonValue value = *myIterator;
			list.push_back(value);
			toCbor(variant, value);

			myIterator++;
		} while (myIterator != myIteratorEnd);

		variant.setValue(list);
	}
	else if (inValue.isObject())
	{
		QJsonObject obj = inValue.toObject();
		QMap<QString, QVariant> map;

		QJsonObject::const_iterator myIterator = obj.constBegin();
		QJsonObject::const_iterator myIteratorEnd = obj.constEnd();
		do
		{
			QString key = myIterator.key();
			QJsonValue value = myIterator.value();
			map.insert(key, value);
			toCbor(variant, value);

			myIterator++;
		} while (myIterator != myIteratorEnd);
		variant.setValue(map);
	}
}

void Class_t::JsonToCbor()
{
	tmpCboreAr.clear();

	QFile fileJsonCbor("cbortest.json");
	if (fileJsonCbor.open(QIODevice::ReadOnly))
	{
		 QVariant variantTop;
		 QJsonDocument json = QJsonDocument::fromJson(fileJsonCbor.readAll());

		 if(json.isArray())
		 {
		 	QJsonArray arr = json.array();
		 	toCbor(variantTop, arr);
		 }
		 else if(json.isObject())
		 {
		 	QJsonObject obj = json.object();
		 	toCbor(variantTop, obj);
		 }

		 QCborValue val = QCborValue::fromVariant(variantTop);

		// How to transfer by 3 str of code:
		/*QVariant var;
		QVariant json = QJsonDocument::fromJson(fileJsonCbor.readAll()).toVariant();
		QCborValue val = QCborValue::fromVariant(json);
		tmpCboreAr = val.toCbor();*/
	}
	qDebug() << "tmpCboreAr" << tmpCboreAr; //////////////////////////////////////
}