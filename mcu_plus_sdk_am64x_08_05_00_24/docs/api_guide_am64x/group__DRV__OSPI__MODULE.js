var group__DRV__OSPI__MODULE =
[
    [ "Generic NOR Flash API for single pin mode", "group__DRV__OSPI__NOR__FLASH__API__MODULE.html", "group__DRV__OSPI__NOR__FLASH__API__MODULE" ],
    [ "ospi/v0/ospi.h", "ospi_2v0_2ospi_8h.html", null ],
    [ "OSPI_Transaction", "structOSPI__Transaction.html", [
      [ "count", "structOSPI__Transaction.html#aac78dd1999f55008428a3c5abe2a51d8", null ],
      [ "buf", "structOSPI__Transaction.html#a183f008dafe489762d560df6e31591c3", null ],
      [ "addrOffset", "structOSPI__Transaction.html#ac14c53d246bad740262e85705c03d17b", null ],
      [ "status", "structOSPI__Transaction.html#a70fe4edf0aa4ccf6afa90e21791c1e89", null ],
      [ "transferTimeout", "structOSPI__Transaction.html#a0a7a18bffd3c922474868c7d652ab261", null ]
    ] ],
    [ "OSPI_WriteCmdParams", "structOSPI__WriteCmdParams.html", [
      [ "cmd", "structOSPI__WriteCmdParams.html#a3296bd3b47a97fada6c7e5fca5d87244", null ],
      [ "cmdAddr", "structOSPI__WriteCmdParams.html#a38571f3e238628d80185f65508089778", null ],
      [ "numAddrBytes", "structOSPI__WriteCmdParams.html#ade975e72ead256e190426564612ce0aa", null ],
      [ "txDataBuf", "structOSPI__WriteCmdParams.html#a5fa3041859654f7b9e437c102ef9afcc", null ],
      [ "txDataLen", "structOSPI__WriteCmdParams.html#aab61472aef202901b7b9b0d56ccf69f6", null ]
    ] ],
    [ "OSPI_ReadCmdParams", "structOSPI__ReadCmdParams.html", [
      [ "cmd", "structOSPI__ReadCmdParams.html#a44bdb7aa2a1e2fe7b36d5e268324bffb", null ],
      [ "cmdAddr", "structOSPI__ReadCmdParams.html#aed28a295de7e4e59b2748e12ac04ee38", null ],
      [ "numAddrBytes", "structOSPI__ReadCmdParams.html#a4ede507eb21b4e63c0004f6752158533", null ],
      [ "dummyBits", "structOSPI__ReadCmdParams.html#a1eec079db8ec9e1720711af372262c00", null ],
      [ "rxDataBuf", "structOSPI__ReadCmdParams.html#afa9587baa9c6954373abe02713fed04f", null ],
      [ "rxDataLen", "structOSPI__ReadCmdParams.html#aab6bc8d955215c21d43441cfb1d685db", null ]
    ] ],
    [ "OSPI_AddrRegion", "structOSPI__AddrRegion.html", [
      [ "regionStartAddr", "structOSPI__AddrRegion.html#a95a0aac54d418b8ffc31bf96d95008e7", null ],
      [ "regionSize", "structOSPI__AddrRegion.html#a63d82db756889a099b5ebd740b5dd2ea", null ]
    ] ],
    [ "OSPI_Params", "structOSPI__Params.html", [
      [ "ospiDmaChIndex", "structOSPI__Params.html#ab2cb12e46fa26cd4ecf7077f8345f218", null ]
    ] ],
    [ "OSPI_Attrs", "structOSPI__Attrs.html", [
      [ "baseAddr", "structOSPI__Attrs.html#a712bae2c500bef1f5d04c6a15b4457ee", null ],
      [ "dataBaseAddr", "structOSPI__Attrs.html#a2758e32ddbddeb1e3127f545aecfbcbc", null ],
      [ "inputClkFreq", "structOSPI__Attrs.html#a899a378953a89c69645e3598f40da22c", null ],
      [ "intrNum", "structOSPI__Attrs.html#a8c02ae12116e6db9e5baceec960c7530", null ],
      [ "intrEnable", "structOSPI__Attrs.html#a03d4b7a0e5ec7ff904f365283b9e748e", null ],
      [ "intrPriority", "structOSPI__Attrs.html#a9b98105b59461cc25feea7ce38dab7df", null ],
      [ "dmaEnable", "structOSPI__Attrs.html#a099339f06c695c8e9bc62b88c5489b35", null ],
      [ "phyEnable", "structOSPI__Attrs.html#a940fd1e5fee67d52ecf27643d2655a81", null ],
      [ "dacEnable", "structOSPI__Attrs.html#a4652a8026110503b11eed62485c0a887", null ],
      [ "frmFmt", "structOSPI__Attrs.html#a7597d6f446c7e153587684f56209d96a", null ],
      [ "devDelays", "structOSPI__Attrs.html#a3d6fd839b2531bcb6c34aef45a39ae9d", null ],
      [ "chipSelect", "structOSPI__Attrs.html#a7f9216b376b14b8d954126b6011b5e67", null ],
      [ "decChipSelect", "structOSPI__Attrs.html#a6663207c76e3abe9f8013d9cd26859f2", null ],
      [ "baudRateDiv", "structOSPI__Attrs.html#ae957c3b4d924a730b7006f920b904060", null ],
      [ "dmaRestrictedRegions", "structOSPI__Attrs.html#a2335c7d39e1e96078e2e6392b07b4b1d", null ]
    ] ],
    [ "OSPI_Object", "structOSPI__Object.html", [
      [ "handle", "structOSPI__Object.html#a47550c4f636ed8ef183fda4dba8fa0eb", null ],
      [ "transferMode", "structOSPI__Object.html#a23d39da785d916da072352ca954083ea", null ],
      [ "protocol", "structOSPI__Object.html#a34f10fc03a9a4054f03ab6d4caf7a79a", null ],
      [ "rdDummyCycles", "structOSPI__Object.html#a70b2c4bb13a89348ee321a7eaaf84efb", null ],
      [ "cmdDummyCycles", "structOSPI__Object.html#a0a18203b1f7f379c94ea6cdb3797b0ae", null ],
      [ "rdDataCapDelay", "structOSPI__Object.html#a6b05a0eeda1e42125a7cb3fc35a44eb3", null ],
      [ "phyRdDataCapDelay", "structOSPI__Object.html#a02614addf9bca944d810221b0736bac9", null ],
      [ "numAddrBytes", "structOSPI__Object.html#aab98d680998ac8e5004a9f7e8fbf907d", null ],
      [ "cmdExtType", "structOSPI__Object.html#af34247cac35cc2c9b3d124f5609916a1", null ],
      [ "isOpen", "structOSPI__Object.html#a82e05007313008385b49d4bac5764cb2", null ],
      [ "lockObj", "structOSPI__Object.html#ac59e394de833441bb1e9d28db10d6e5b", null ],
      [ "transferSemObj", "structOSPI__Object.html#a4fdf482885a4292639c9f81ddf4704e1", null ],
      [ "hwiObj", "structOSPI__Object.html#a7dfdf17f1f5b6a58c235095c4ed0e52a", null ],
      [ "phyEnableSuccess", "structOSPI__Object.html#a60620705e07c20cbe74c6484b938fe64", null ],
      [ "currTrans", "structOSPI__Object.html#a7a929cbe21ad3e4bdd30f941ec124ae2", null ],
      [ "ospiDmaHandle", "structOSPI__Object.html#a79aff6bb896caaa89513af0ae842f2a7", null ]
    ] ],
    [ "OSPI_Config", "structOSPI__Config.html", [
      [ "attrs", "structOSPI__Config.html#afae849ff69bd1a26ef1f9d574316497c", null ],
      [ "object", "structOSPI__Config.html#a23a4360ecf2230ba18ff20956ce14274", null ]
    ] ],
    [ "OSPI_TRANSFER_COMPLETED", "group__DRV__OSPI__MODULE.html#ga948ce22a66ab874258bed621c990df72", null ],
    [ "OSPI_TRANSFER_STARTED", "group__DRV__OSPI__MODULE.html#ga8482f3d9ad125c31a3580e5dd0f88b7b", null ],
    [ "OSPI_TRANSFER_CANCELLED", "group__DRV__OSPI__MODULE.html#gac019dac1e7917fd6351ac4840e763195", null ],
    [ "OSPI_TRANSFER_FAILED", "group__DRV__OSPI__MODULE.html#ga7d6bd3a34c82f66d9c1cb10480793aaa", null ],
    [ "OSPI_TRANSFER_CSN_DEASSERT", "group__DRV__OSPI__MODULE.html#gaf383c0431b9a1e5a4548e496e34a07b0", null ],
    [ "OSPI_TRANSFER_TIMEOUT", "group__DRV__OSPI__MODULE.html#gabf886a11af66b463ed93bf674140e149", null ],
    [ "OSPI_TRANSFER_MODE_BLOCKING", "group__DRV__OSPI__MODULE.html#ga91f0d750162c0ef0529fc3c4657ee3a3", null ],
    [ "OSPI_TRANSFER_MODE_CALLBACK", "group__DRV__OSPI__MODULE.html#gafe5b7c84a86e56c3ad7f73f5e7153a52", null ],
    [ "OSPI_TRANSFER_MODE_POLLING", "group__DRV__OSPI__MODULE.html#ga7c33406bc45d1604343695af8a3226ab", null ],
    [ "OSPI_FF_POL0_PHA0", "group__DRV__OSPI__MODULE.html#gad33321c4d3c0e127cc9dbc5150a5d946", null ],
    [ "OSPI_FF_POL0_PHA1", "group__DRV__OSPI__MODULE.html#ga914c31e0aa9d3128653c66fb649b9a72", null ],
    [ "OSPI_FF_POL1_PHA0", "group__DRV__OSPI__MODULE.html#ga8bd7139781e775813b256f0cb7a0a007", null ],
    [ "OSPI_FF_POL1_PHA1", "group__DRV__OSPI__MODULE.html#gab72a37197e96fc7690b1dfdb751e1cca", null ],
    [ "OSPI_CMD_INVALID_OPCODE", "group__DRV__OSPI__MODULE.html#ga41969ba56f8290fa5257258a3a256f67", null ],
    [ "OSPI_CMD_INVALID_DUMMY", "group__DRV__OSPI__MODULE.html#gaa8bac9ef393d38a227ade00fdac952a5", null ],
    [ "OSPI_CMD_INVALID_ADDR", "group__DRV__OSPI__MODULE.html#gaaef3b5598e9c6578bb727f76e4633b46", null ],
    [ "OSPI_CMD_EXT_TYPE_REPEAT", "group__DRV__OSPI__MODULE.html#gabfc137bf2576fc0426a59f5d718a9a72", null ],
    [ "OSPI_CMD_EXT_TYPE_INVERSE", "group__DRV__OSPI__MODULE.html#ga928e0281d6364ec0942501d530e6de75", null ],
    [ "OSPI_CMD_EXT_TYPE_NONE", "group__DRV__OSPI__MODULE.html#ga61928eb243bab120533b9f9a0395dd7b", null ],
    [ "OSPI_NOR_PROTOCOL", "group__DRV__OSPI__MODULE.html#ga06537aa23596702c0565fa7d18d0d1e3", null ],
    [ "OSPI_NOR_PROTOCOL_INVALID", "group__DRV__OSPI__MODULE.html#ga23709aa421bf0c8b7b2f68cf0589f6d6", null ],
    [ "OSPI_CS0", "group__DRV__OSPI__MODULE.html#gadfd42de3f9f3f1aaa42329ab54226f15", null ],
    [ "OSPI_CS1", "group__DRV__OSPI__MODULE.html#ga79d77e39f6bd7e3760400dbd9c9517a0", null ],
    [ "OSPI_CS2", "group__DRV__OSPI__MODULE.html#ga648138c29d9a9eac6698a2b4e3aa9ede", null ],
    [ "OSPI_CS3", "group__DRV__OSPI__MODULE.html#ga50d149b2fccd6e812bb0dd6f59a7b94d", null ],
    [ "OSPI_CHIP_SELECT", "group__DRV__OSPI__MODULE.html#ga79a83a57a9d53e16d427da7085484340", null ],
    [ "OSPI_DECODER_SELECT4", "group__DRV__OSPI__MODULE.html#ga9f104789738763e778992459be823072", null ],
    [ "OSPI_DECODER_SELECT16", "group__DRV__OSPI__MODULE.html#gae589b4ecf0e83feaf138aa818f87a439", null ],
    [ "OSPI_Handle", "group__DRV__OSPI__MODULE.html#gaf94e42c41b084bb2f3c9cc4409213188", null ],
    [ "OSPI_init", "group__DRV__OSPI__MODULE.html#ga53c8b806a48ecdb9d3b8d17c36271819", null ],
    [ "OSPI_deinit", "group__DRV__OSPI__MODULE.html#ga4bf4c5e9986edf6545527bc9690c5f66", null ],
    [ "OSPI_Params_init", "group__DRV__OSPI__MODULE.html#ga2a6c4001017c758a627f1cd78115e120", null ],
    [ "OSPI_open", "group__DRV__OSPI__MODULE.html#gaf74c4b8836a43513f0b2c0a7893c02da", null ],
    [ "OSPI_close", "group__DRV__OSPI__MODULE.html#ga6be5558d3c8085469468d6f8e7c697f2", null ],
    [ "OSPI_getHandle", "group__DRV__OSPI__MODULE.html#gac9779c195e27fabcc62aaa1045e9b05f", null ],
    [ "OSPI_readDirect", "group__DRV__OSPI__MODULE.html#gadf95ceab8b2d8466b6dbc94bb3ab0a56", null ],
    [ "OSPI_readIndirect", "group__DRV__OSPI__MODULE.html#ga8f67a450022d20e50cc726084b0f36af", null ],
    [ "OSPI_writeDirect", "group__DRV__OSPI__MODULE.html#gab7a73f169d42c9aa17ac75f40af3db4d", null ],
    [ "OSPI_writeIndirect", "group__DRV__OSPI__MODULE.html#ga32d6168dcfa566f8fc11f6e2f76f71e6", null ],
    [ "OSPI_readCmd", "group__DRV__OSPI__MODULE.html#ga068459dfaa30ff756183498515daa7ac", null ],
    [ "OSPI_writeCmd", "group__DRV__OSPI__MODULE.html#ga0decd6fd76069512d26890d90fc12e78", null ],
    [ "OSPI_Transaction_init", "group__DRV__OSPI__MODULE.html#ga66014cbd24d8c287c94846a049d9da68", null ],
    [ "OSPI_ReadCmdParams_init", "group__DRV__OSPI__MODULE.html#gac0fea092fe6e546dd99afbac73aa1cb5", null ],
    [ "OSPI_WriteCmdParams_init", "group__DRV__OSPI__MODULE.html#ga7e7fe11788a7985ea5075fa1f628b437", null ],
    [ "OSPI_getInputClk", "group__DRV__OSPI__MODULE.html#gac2f47121c92b9d033b0b1c7540d0c8d7", null ],
    [ "OSPI_isDacEnable", "group__DRV__OSPI__MODULE.html#gacfdbd06fc0504e3a3eed1c31a2d42fd3", null ],
    [ "OSPI_isDmaEnable", "group__DRV__OSPI__MODULE.html#gaee5fdd9553f4bd0223b85abf6261d15f", null ],
    [ "OSPI_isIntrEnable", "group__DRV__OSPI__MODULE.html#ga77f5a9e01741701f6330bdedf85b7321", null ],
    [ "OSPI_isPhyEnable", "group__DRV__OSPI__MODULE.html#ga3be4b0de1b7d5267d614cbec80e86c37", null ],
    [ "OSPI_isDtrEnable", "group__DRV__OSPI__MODULE.html#ga353a685bc155f7ae996f558f3b529317", null ],
    [ "OSPI_enableDDR", "group__DRV__OSPI__MODULE.html#ga0de448ce4c70f2ee98af6707356bcbe5", null ],
    [ "OSPI_enableSDR", "group__DRV__OSPI__MODULE.html#ga71774ed21246ab68c4e27f01c466c809", null ],
    [ "OSPI_enableDdrRdCmds", "group__DRV__OSPI__MODULE.html#ga0137399fd2f537445956e40ec0ae7257", null ],
    [ "OSPI_setRdDataCaptureDelay", "group__DRV__OSPI__MODULE.html#ga9421b052a58bf3cbe326b5d74919a877", null ],
    [ "OSPI_setNumAddrBytes", "group__DRV__OSPI__MODULE.html#ga89e702ca1f019b5d88fa8bb2c030ab38", null ],
    [ "OSPI_setDeviceSize", "group__DRV__OSPI__MODULE.html#ga4c8d90f7305e72b77489e2fda76565cd", null ],
    [ "OSPI_setCmdDummyCycles", "group__DRV__OSPI__MODULE.html#gab738b402ed88d112d1414dc33b22ab85", null ],
    [ "OSPI_setReadDummyCycles", "group__DRV__OSPI__MODULE.html#ga68a204e2a560c924a71c03d4b372f5ad", null ],
    [ "OSPI_setPhyEnableSuccess", "group__DRV__OSPI__MODULE.html#ga99056e6c013a944620dd45cc6d69a80b", null ],
    [ "OSPI_setModeBits", "group__DRV__OSPI__MODULE.html#ga4c718fa1c185434d2e84146005883447", null ],
    [ "OSPI_enableModeBitsCmd", "group__DRV__OSPI__MODULE.html#ga9f8a3ee5c33cf3ca9e02319456d71330", null ],
    [ "OSPI_enableModeBitsRead", "group__DRV__OSPI__MODULE.html#gaafe5e8aa4ae563bd4c067dfca4b68d44", null ],
    [ "OSPI_getPhyEnableSuccess", "group__DRV__OSPI__MODULE.html#gab9fa291138465098c9e98ac074849e4c", null ],
    [ "OSPI_cmdModeBitSet", "group__DRV__OSPI__MODULE.html#ga1fd1161bd5604bf6f1d407939482c955", null ],
    [ "OSPI_rdModeBitSet", "group__DRV__OSPI__MODULE.html#ga2f6f3296f315d5c519cb23757dbb26e8", null ],
    [ "OSPI_getProtocol", "group__DRV__OSPI__MODULE.html#ga6f1745f40bd55489e6b664ffeebf5273", null ],
    [ "OSPI_setProtocol", "group__DRV__OSPI__MODULE.html#ga389adec0e7fc0966af6a47c514826c52", null ],
    [ "OSPI_setDualOpCodeMode", "group__DRV__OSPI__MODULE.html#gac799c521f13c9ba7d42d46a9e8425e1e", null ],
    [ "OSPI_clearDualOpCodeMode", "group__DRV__OSPI__MODULE.html#gabbc54d5d48c72e6f1ab0ede20c8a77fb", null ],
    [ "OSPI_setXferOpCodes", "group__DRV__OSPI__MODULE.html#gac04d05439c8e99fc638e397abdfd801b", null ],
    [ "OSPI_setCmdExtType", "group__DRV__OSPI__MODULE.html#ga50c51cccae6641360d76408715e713f0", null ],
    [ "OSPI_enableDacMode", "group__DRV__OSPI__MODULE.html#ga038e066fc28090e138363c859b027869", null ],
    [ "OSPI_disableDacMode", "group__DRV__OSPI__MODULE.html#ga55d02050f79b0efdc5dcddcdbc94bfd8", null ],
    [ "OSPI_getFlashDataBaseAddr", "group__DRV__OSPI__MODULE.html#gac85aad283f153e3c2a4a717660ef92d8", null ],
    [ "OSPI_phyTuneDDR", "group__DRV__OSPI__MODULE.html#gafd38c99387c090ae11f944bfdae39354", null ],
    [ "OSPI_phyTuneGrapher", "group__DRV__OSPI__MODULE.html#gadfe1b8c64f92cae7211210a47d831c6f", null ],
    [ "OSPI_phyGetTuningData", "group__DRV__OSPI__MODULE.html#gac30f489140d0ff25405969f349a754f4", null ],
    [ "OSPI_phyReadAttackVector", "group__DRV__OSPI__MODULE.html#ga3af2d81bc9e28cb9b7ca4835ff7c1716", null ],
    [ "OSPI_enablePhy", "group__DRV__OSPI__MODULE.html#gac1e3e646e56fade0dae155b15145ed09", null ],
    [ "OSPI_disablePhy", "group__DRV__OSPI__MODULE.html#ga6da77149b2e1ccb884117269bb8da65d", null ],
    [ "OSPI_enablePhyPipeline", "group__DRV__OSPI__MODULE.html#ga28a185b5eeed7a8dd56f35b116a44695", null ],
    [ "OSPI_disablePhyPipeline", "group__DRV__OSPI__MODULE.html#ga6d9b5b1d5eeedfe5fb06d2e38d096c8f", null ],
    [ "gOspiConfig", "group__DRV__OSPI__MODULE.html#ga9dd730ce6cdd6b12a70d504c407d83ac", null ],
    [ "gOspiConfigNum", "group__DRV__OSPI__MODULE.html#gad2886a408f41de0874d6cedf9696f363", null ]
];