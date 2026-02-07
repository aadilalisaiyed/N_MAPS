import express from "express";
import axios from "axios";

const router = express.Router();

// POST → send text to ML server
router.post("/analyze", async (req, res) => {
  try {
    const { message } = req.body;

    if (!message) {
      return res.status(400).json({ error: "Message is required" });
    }

    // call Python ML server
    const mlResponse = await axios.post(
      "http://127.0.0.1:8000/predict",
      { message }
    );

    // send ML result back
    res.json({
      original_message: message,
      prediction: mlResponse.data.prediction,
      confidence: mlResponse.data.confidence
    });

  } catch (error) {
    console.error(error.message);
    res.status(500).json({ error: "ML server not reachable" });
  }
});

export default router;
