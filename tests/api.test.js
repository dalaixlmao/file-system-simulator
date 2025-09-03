const request = require('supertest');
const app = require('../src/app');
const Storage = require('../src/storage/Storage');

describe('File System API', () => {
  // Reset storage before each test
  beforeEach(() => {
    Storage.resetInstance();
  });

  describe('Files API', () => {
    test('Should create a new file', async () => {
      const res = await request(app)
        .post('/api/files')
        .send({ name: 'test.txt' });
      
      expect(res.statusCode).toBe(201);
      expect(res.body.success).toBe(true);
      expect(res.body.data.name).toBe('test.txt');
    });
    
    test('Should not create a file without name', async () => {
      const res = await request(app)
        .post('/api/files')
        .send({});
      
      expect(res.statusCode).toBe(400);
      expect(res.body.success).toBe(false);
    });
    
    test('Should get all files', async () => {
      // Create a file first
      await request(app)
        .post('/api/files')
        .send({ name: 'test1.txt' });
      
      await request(app)
        .post('/api/files')
        .send({ name: 'test2.txt' });
      
      const res = await request(app).get('/api/files');
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      expect(res.body.data.length).toBe(2);
      expect(res.body.data[0].name).toBe('test1.txt');
      expect(res.body.data[1].name).toBe('test2.txt');
    });
    
    test('Should write content to a file', async () => {
      // Create a file first
      await request(app)
        .post('/api/files')
        .send({ name: 'test.txt' });
      
      const content = 'Hello, World!';
      const res = await request(app)
        .put('/api/files/test.txt/content')
        .send({ content });
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      expect(res.body.data.content).toBe(content);
    });
    
    test('Should get file details', async () => {
      // Create a file first
      await request(app)
        .post('/api/files')
        .send({ name: 'test.txt' });
      
      // Write content
      const content = 'Hello, World!';
      await request(app)
        .put('/api/files/test.txt/content')
        .send({ content });
      
      const res = await request(app).get('/api/files/test.txt');
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      expect(res.body.data.name).toBe('test.txt');
      expect(res.body.data.content).toBe(content);
    });
    
    test('Should delete a file', async () => {
      // Create a file first
      await request(app)
        .post('/api/files')
        .send({ name: 'test.txt' });
      
      const res = await request(app).delete('/api/files/test.txt');
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      
      // Verify file was deleted
      const checkRes = await request(app).get('/api/files/test.txt');
      expect(checkRes.statusCode).toBe(404);
    });
  });

  describe('Folders API', () => {
    test('Should create a new folder', async () => {
      const res = await request(app)
        .post('/api/folders')
        .send({ name: 'testFolder' });
      
      expect(res.statusCode).toBe(201);
      expect(res.body.success).toBe(true);
      expect(res.body.data.name).toBe('testFolder');
    });
    
    test('Should not create a folder without name', async () => {
      const res = await request(app)
        .post('/api/folders')
        .send({});
      
      expect(res.statusCode).toBe(400);
      expect(res.body.success).toBe(false);
    });
    
    test('Should get all folders', async () => {
      // Create folders first
      await request(app)
        .post('/api/folders')
        .send({ name: 'folder1' });
      
      await request(app)
        .post('/api/folders')
        .send({ name: 'folder2' });
      
      const res = await request(app).get('/api/folders');
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      expect(res.body.data.length).toBe(2);
      expect(res.body.data[0].name).toBe('folder1');
      expect(res.body.data[1].name).toBe('folder2');
    });
    
    test('Should get folder details', async () => {
      // Create a folder first
      await request(app)
        .post('/api/folders')
        .send({ name: 'testFolder' });
      
      const res = await request(app).get('/api/folders/testFolder');
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      expect(res.body.data.name).toBe('testFolder');
    });
    
    test('Should delete a folder', async () => {
      // Create a folder first
      await request(app)
        .post('/api/folders')
        .send({ name: 'testFolder' });
      
      const res = await request(app).delete('/api/folders/testFolder');
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      
      // Verify folder was deleted
      const checkRes = await request(app).get('/api/folders/testFolder');
      expect(checkRes.statusCode).toBe(404);
    });
    
    test('Should list contents of current folder', async () => {
      // Create a file and folder
      await request(app)
        .post('/api/files')
        .send({ name: 'test.txt' });
      
      await request(app)
        .post('/api/folders')
        .send({ name: 'testFolder' });
      
      const res = await request(app).get('/api/folders/content');
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      expect(res.body.data.length).toBe(2);
      
      // Check that we have both a file and a folder
      const fileItem = res.body.data.find(item => item.type === 'file');
      const folderItem = res.body.data.find(item => item.type === 'folder');
      
      expect(fileItem).toBeDefined();
      expect(folderItem).toBeDefined();
      expect(fileItem.name).toBe('test.txt');
      expect(folderItem.name).toBe('testFolder');
    });
  });

  describe('Navigation API', () => {
    test('Should get current path', async () => {
      const res = await request(app).get('/api/navigation/path');
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      expect(res.body.data.path).toBeDefined();
    });
    
    test('Should change directory', async () => {
      // Create a folder first
      await request(app)
        .post('/api/folders')
        .send({ name: 'testFolder' });
      
      const res = await request(app)
        .post('/api/navigation/cd')
        .send({ folderName: 'testFolder' });
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      expect(res.body.data.path).toContain('testFolder');
      
      // Go back to parent
      const backRes = await request(app)
        .post('/api/navigation/cd')
        .send({ folderName: '..' });
      
      expect(backRes.statusCode).toBe(200);
      expect(backRes.body.success).toBe(true);
      expect(backRes.body.data.path).not.toContain('testFolder');
    });
    
    test('Should get file system tree', async () => {
      // Create a file and folder
      await request(app)
        .post('/api/files')
        .send({ name: 'test.txt' });
      
      await request(app)
        .post('/api/folders')
        .send({ name: 'testFolder' });
      
      const res = await request(app).get('/api/navigation/tree');
      
      expect(res.statusCode).toBe(200);
      expect(res.body.success).toBe(true);
      expect(res.body.data).toBeDefined();
      expect(res.body.data.children).toBeDefined();
      expect(res.body.data.children.length).toBe(2);
      
      // Check that we have both a file and a folder in the tree
      const fileNode = res.body.data.children.find(node => node.type === 'file');
      const folderNode = res.body.data.children.find(node => node.type === 'folder');
      
      expect(fileNode).toBeDefined();
      expect(folderNode).toBeDefined();
      expect(fileNode.name).toBe('test.txt');
      expect(folderNode.name).toBe('testFolder');
      expect(folderNode.children).toBeDefined();
    });
  });
});