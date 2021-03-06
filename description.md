# IIR课程项目

## 目的

初步目标：建立Sherlock Holmes的人物-事件检索系统

## 步骤

- 提取人名
  - 方法1：使用手工编写的规则进行匹配（容易错漏）
  - 方法2：使用python的NLTK库或SpaCy库进行匹配
    - 参考：[Quora](https://www.quora.com/Whats-a-good-algorithm-for-people-name-detection-How-can-I-tell-if-a-string-contains-a-persons-name)
- 提取篇章：文本提供了章节目录，而正文中的章节大标题用全大写字母表示。可以据此将各个章节（即各个事件）分开。
- 信息挖掘
  - 事件相关
    - 关键词/句：tf-idf计算单词权重；摘要生成算法生成关键句
    - 通过对上述结果进行分析，可能提取出每个事件发生的时间、地点等
  - 人物信息抽取
    - 不同人物在同一事件的共现情况
    - 某个人物在某个事件中的活跃程度
      - 需要讨论一些指标，比如某个人物在事件中的出现次数、说话次数与话语长度，该人物相关的句子与事件关键词/关键句的重叠度等等
    - 某个人物在不同事件中多次出现的情况
- 设计检索系统
  - 查询方法
    - 给定一个查询集合，集合中包含若干关键词。检索系统需要给出与这些关键词相关的人物或事件集合。如果设置了某些参数，检索系统还应该给出这些人物或事件的详细信息。
    - 如果查询的是一个句子，可以对这个句子进行一些预处理提取查询需要的key（例如分词后去除停用词，或进行语义分析找关键词等）
  - 维护方法
    - 该检索系统中维护了许多不同的信息（人物-事件映射、事件-事件信息映射、人物-人物信息映射等）。虽然Sherlock Holmes的文本量不大，这些信息都可以存在内存中，但考虑到实际应用意义，可以实现利用外存的检索系统，将大部分信息存储在磁盘上，每次查询时仅将必要的信息加载进内存。

## 完善

  1. 可视化：将人物/事件的关系使用第三方库绘制成直观的图片，或可交互的html。
  2. 前端：设计调用接口，实现基于html/js的前端
  3. 评价指标：基于功能/性能两个方面对检索系统进行评价。
      1. 功能性：利用precision、recall、mAP等指标。问题：是否需要构建ground-truth？如何有效构建ground-truth（除人工标注以外的方法）
      2. 性能：记录检索系统占用的内存、每次查询所用的时间等指标
  4. 使检索系统能够处理除简单词句以外的任意询问（需要对询问进行更细致的预处理）
  5. 使检索系统在特定情况下能以自然语言的形式给出检索结果，而非简单地罗列事件信息（可能需要预训练的语言模型）。
